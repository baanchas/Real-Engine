#include "repch.h"
#include "ObjectLoader.h"

namespace RealEngine {

	void ModelLoader::LoadObjectFromOBJ(const std::string& fileName, Mesh& mesh)
	{

		std::vector<glm::vec3> vertexPositions;
		std::vector<glm::vec4> vertexColor;
		std::vector<glm::vec2> vertexTexCoords;
		std::vector<glm::vec3> vertexNormals;

		std::vector<uint32_t> vertexPositionsIndices;
		std::vector<uint32_t> vertexTexCoordIndicies;
		std::vector<uint32_t> vertexNormalsIndices;

		//std::vector<Vertex> verticesToReturn;
		//std::vector<uint32_t> indicesToReturn;

		std::ifstream in(fileName + ".obj");

		std::stringstream ss;
		std::string line;
		std::string prefix;
		std::string material;
		glm::vec3 tempPos;
		glm::vec3 tempColor = glm::vec3{1.0f, 1.0f, 1.0f};
		glm::vec2 tempTexCoord;

		uint32_t tempFaceVariable = 0;

		int vertexCounter = 0;

		if (!in.is_open())
		{
			ENGINE_ERROR("Could not open file {0}", fileName);
		}

		while (std::getline(in, line))
		{
			ss.clear();
			ss.str(line);
			ss >> prefix;

			if (prefix == "#")
			{

			}
			else if (prefix == "o")
			{

			}
			else if (prefix == "s")
			{

			}
			else if (prefix == "usemtl")
			{
				ss >> material;
				bool canCheckForColor = false;

				std::ifstream mtl(fileName + ".mtl");

				if (mtl.is_open())
				{
					std::string tempLine;
					std::stringstream strstr;
					std::string materialToCompare;
					std::string prefixToComaper;

					while (getline(mtl, tempLine))
					{

						strstr.clear();
						strstr.str(tempLine);
						strstr >> prefix;

						if (prefix == "newmtl")
						{
							strstr >> materialToCompare;
							if (material == materialToCompare)
							{
								canCheckForColor = true;
							}
						}

						if (canCheckForColor)
						{
							if (prefix == "Kd")
							{
								strstr >> tempColor.x >> tempColor.y >> tempColor.z;
								canCheckForColor = false;
							}
						}
					}
				}

				mtl.close();
			}
			else if (prefix == "v") 
			{
				ss >> tempPos.x >> tempPos.y >> tempPos.z;
				vertexPositions.push_back(tempPos);	
			}
			else if (prefix == "vt")
			{
				ss >> tempTexCoord.x >> tempTexCoord.y;
				vertexTexCoords.push_back(tempTexCoord);
			}
			else if (prefix == "vn")
			{
				ss >> tempPos.x >> tempPos.y >> tempPos.z;
				vertexNormals.push_back(tempPos);
			}
			else if (prefix == "f")
			{
				std::vector<uint32_t> tempIndices;
				int counter = 0;
				while (ss >> tempFaceVariable)
				{
					if (counter == 0)
					{
						vertexPositionsIndices.push_back(tempFaceVariable);
						vertexColor.push_back(glm::vec4{ tempColor, 1.0f });
						tempIndices.push_back(tempFaceVariable);
						vertexCounter++;
					}
					else if (counter == 1)
					{
						vertexTexCoordIndicies.push_back(tempFaceVariable);
					}
					else if (counter == 2)
					{
						vertexNormalsIndices.push_back(tempFaceVariable);
					}

					if (ss.peek() == '/')
					{
						++counter;
						ss.ignore(1, '/');
					}
					else if (ss.peek() == ' ')
					{
						++counter;
						ss.ignore(1, ' ');
					}

					if (counter > 2)
					{
						counter = 0;
					}
				}

				int i = 1;

				while(i < tempIndices.size() - 1)
				{
					mesh.GetIndices().push_back(vertexCounter - tempIndices.size());
					mesh.GetIndices().push_back(vertexCounter - tempIndices.size() + i);
					mesh.GetIndices().push_back(vertexCounter - tempIndices.size() + i + 1);
					i++;
				}

			}
		}
	
		for (size_t i = 0; i < vertexPositionsIndices.size(); ++i)
		{
			Vertex vertex;
			mesh.GetVertices().push_back(vertex);
			mesh.GetVertices()[i].Position = vertexPositions[vertexPositionsIndices[i] - 1];
			mesh.GetVertices()[i].TexCoord = vertexTexCoords[vertexTexCoordIndicies[i] - 1];
			mesh.GetVertices()[i].Normal = vertexNormals[vertexNormalsIndices[i] - 1];
			mesh.GetVertices()[i].Color = vertexColor[i];
		}
		ENGINE_INFO("{0} Vertices has been loaded from ""{1}"" file.", vertexPositionsIndices.size(), fileName);
	}


	void ModelLoader::LoadObjectFromFBX(const std::string& path, Mesh& myMesh)
	{
		// Create the FBX SDK manager
		FbxManager* lSdkManager = FbxManager::Create();

		// Create an IOSettings object.
		FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
		lSdkManager->SetIOSettings(ios);

		// ... Configure the FbxIOSettings object ...

		// Create an importer.
		FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

		// Declare the path and filename of the file containing the scene.
		// In this case, we are assuming the file is in the same directory as the executable.
		//std::string lFilename = "file.fbx";

		// Initialize the importer.
		bool lImportStatus = lImporter->Initialize(path.c_str(), -1, lSdkManager->GetIOSettings());

		if (!lImportStatus) {
			printf("Call to FbxImporter::Initialize() failed.\n");
			printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
			exit(-1);
		}

		FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

		// Import the contents of the file into the scene.
		lImporter->Import(lScene);

		lImporter->Destroy();

		// Get the root node of the scene.
		FbxNode* lRootNode = lScene->GetRootNode();
		LoadNode(lRootNode, myMesh);
	
	}

	void ModelLoader::LoadNode(FbxNode* node, Mesh& myMesh)
	{
		/*int materialCount = node->GetMaterialCount();
		for (int i = 0; i < materialCount; i++)
		{
			FbxSurfaceMaterial* material = node->GetMaterial(i);
			if (material != nullptr && !material->GetUserDataPtr())
			{
				FbxAutoPtr<MaterialCache> materialCache(new MaterialCache());
				if (materialCache->Initialize(material))
					material->SetUserDataPtr(materialCache.Release());
			}
		}*/

		FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
		if (nodeAttribute)
		{
			// Bake mesh as VBO(vertex buffer object) into GPU.
			if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
			{
				FbxMesh* mesh = node->GetMesh();
				if (mesh && !mesh->GetUserDataPtr())
					Initialize(mesh, myMesh);
			}
			// Bake light properties.
			else if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eLight)
			{
				std::cout << "Note: Lights are currently not supported" << std::endl;
			}
		}

		const int childCount = node->GetChildCount();
		if (childCount != 0)
		{
			for (int i = 0; i < childCount; i++)
				LoadNode(node->GetChild(i), myMesh);
		}


	}

	bool ModelLoader::Initialize(FbxMesh* mesh, Mesh& myMesh)
	{
		if (!mesh->GetNode())
			return false;

		const int polygonCount = mesh->GetPolygonCount();
		std::vector<glm::vec3> verticesPositions;

		std::cout << "Importing " << polygonCount << " polygons." << std::endl;
		ProcessControlPoints(mesh, verticesPositions);
		uint32_t triCount = mesh->GetPolygonCount();
		int vertexCounter = 0;
		int* indicesCount = mesh->GetPolygonVertices();
	
		for (uint32_t i = 0; i < triCount; i++)
		{
			glm::vec3 tangent;
			glm::vec3 binormal;
			glm::vec2 uv;

			uint32_t sizeOfPolygon = mesh->GetPolygonSize(i);
			for (uint32_t j = 0; j < sizeOfPolygon; j++)
			{
				Vertex vertex;


				int controlPointIndex = mesh->GetPolygonVertex(i, j);
				glm::vec3& position = verticesPositions[controlPointIndex];
				glm::vec3 normal = ReadNormal(mesh, controlPointIndex, vertexCounter);
				//glm::vec3 binormal = ReadBinormal(mesh, controlPointIndex, vertexCounter);
				//glm::vec3 tangent = ReadTangent(mesh, controlPointIndex, vertexCounter);

				// for (uint t = 0; t < 1; t++) // TODO: Support multiple layers
				glm::vec2 uv = ReadUV(mesh, controlPointIndex, mesh->GetTextureUVIndex(i, j), 0);

				vertex.Position = position;
				vertex.Normal = normal;
				vertex.TexCoord = uv;

				myMesh.GetVertices().push_back(vertex);
			
				vertexCounter++;
		
			}

			int startind = mesh->GetPolygonVertexIndex(i);

			int k = 1;

			while (k < sizeOfPolygon - 1)
			{
				myMesh.GetIndices().push_back(startind);
				myMesh.GetIndices().push_back(startind + k);
				myMesh.GetIndices().push_back(startind + k + 1);
				k++;
			}

		}
		ENGINE_INFO("{0} Vertices has been loaded", vertexCounter);

	}

	void ModelLoader::ProcessControlPoints(const FbxMesh* mesh, std::vector<glm::vec3>& positions)
	{
		const uint32_t count = mesh->GetControlPointsCount();
	
		for (uint32_t i = 0; i < count; i++)
		{
			glm::vec3 position;
			position.x = static_cast<float>(mesh->GetControlPointAt(i).mData[0]);
			position.y = static_cast<float>(mesh->GetControlPointAt(i).mData[1]);
			position.z = static_cast<float>(mesh->GetControlPointAt(i).mData[2]);
			positions.push_back(position);
		}
	}

	glm::vec2 ModelLoader::ReadUV(const FbxMesh* inMesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer)
	{
		if (inUVLayer >= 2 || inMesh->GetElementUVCount() <= inUVLayer)
		{
			//std::cout << "Invalid UV Layer Number" << std::endl;
			glm::vec2 result = glm::vec2{ 1.0f, 1.0f };
			return result;
		}
		else
		{
			const FbxGeometryElementUV* vertexUV = inMesh->GetElementUV(inUVLayer);

			glm::vec2 result;
			switch (vertexUV->GetMappingMode())
			{
			case FbxGeometryElement::eByControlPoint:
				switch (vertexUV->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
				{
					result.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
					result.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
				}
				break;

				case FbxGeometryElement::eIndexToDirect:
				{
					int index = vertexUV->GetIndexArray().GetAt(inCtrlPointIndex);
					result.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
					result.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
				}
				break;
				default:
					std::cout << "Error: Invalid vertex reference mode!" << std::endl;
				}
				break;

			case FbxGeometryElement::eByPolygonVertex:
				switch (vertexUV->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
				case FbxGeometryElement::eIndexToDirect:
				{
					result.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[0]);
					result.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[1]);
				}
				break;
				default:
					std::cout << "Error: Invalid vertex reference mode!" << std::endl;
				}
				break;
			}
			return result;
		}
	}

	glm::vec3 ModelLoader::ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter)
	{
		if (mesh->GetElementNormalCount() < 1)
			std::cout << "No normals!" << std::endl;

		glm::vec3 result;
		const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
		switch (vertexNormal->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			switch (vertexNormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
				result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
				result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
				result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
				result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
				result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
			}
			break;
			default:
				std::cout << "Error: Invalid vertex reference mode!" << std::endl;
			}
			break;

		case FbxGeometryElement::eByPolygonVertex:
			switch (vertexNormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
				result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
				result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexNormal->GetIndexArray().GetAt(vertexCounter);
				result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
				result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
				result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
			}
			break;
			default:
				std::cout << "Error: Invalid vertex reference mode!" << std::endl;
			}
			break;
		}
		return result;
	}
}