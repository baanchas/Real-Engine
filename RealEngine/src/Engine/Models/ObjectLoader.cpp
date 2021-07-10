#include "repch.h"
#include "ObjectLoader.h"

namespace RealEngine {

	std::pair<std::vector<Vertex>, std::vector<uint32_t>> ObjectLoader::LoadObjectFromOBJ(const std::string& fileName)
	{

		std::vector<glm::vec3> vertexPositions;
		std::vector<glm::vec4> vertexColor;
		std::vector<glm::vec2> vertexTexCoords;
		std::vector<glm::vec3> vertexNormals;

		std::vector<uint32_t> vertexPositionsIndices;
		std::vector<uint32_t> vertexTexCoordIndicies;
		std::vector<uint32_t> vertexNormalsIndices;

		std::vector<Vertex> verticesToReturn;
		std::vector<uint32_t> indicesToReturn;

		std::ifstream in(fileName + ".obj");

		std::stringstream ss;
		std::string line;
		std::string prefix;
		std::string material;
		glm::vec3 tempPos;
		glm::vec3 tempColor;
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
					indicesToReturn.push_back(vertexCounter - tempIndices.size());
					indicesToReturn.push_back(vertexCounter - tempIndices.size() + i);
					indicesToReturn.push_back(vertexCounter - tempIndices.size() + i + 1);
					i++;
				}

			}
		}
		ENGINE_INFO(vertexCounter);
				
		verticesToReturn.resize(vertexPositionsIndices.size(), Vertex());

		for (size_t i = 0; i < verticesToReturn.size(); ++i)
		{
			verticesToReturn[i].Position = vertexPositions[vertexPositionsIndices[i] - 1];
			verticesToReturn[i].TexCoord = vertexTexCoords[vertexTexCoordIndicies[i] - 1];
			verticesToReturn[i].Color = vertexColor[i];
		}
		ENGINE_INFO("ObjectLoader::{0} Vertices Loaded from object at \"{1}\"", verticesToReturn.size(), fileName);

		return { verticesToReturn, indicesToReturn };
	}

}