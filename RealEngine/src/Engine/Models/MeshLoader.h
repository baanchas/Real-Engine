#pragma once

#include "OpenGL/VertexBuffer.h"

namespace RealEngine {

	class MeshLoader
	{
	public:
		// OBJ extension func
		static void LoadMeshFromOBJ(const std::string& file_name, Mesh& mesh);

		// FBX extension func
		static void LoadMeshFromFBX(const std::string& path, Mesh& myMesh);

	private:
		// FBX extension func
		static bool Initialize(FbxMesh* mesh, Mesh& myMesh);
		static void LoadNode(FbxNode* node, Mesh& myMesh);
		static void ProcessControlPoints(const FbxMesh* mesh, std::vector<glm::vec3>& positions);
		static glm::vec3 ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter);
		static glm::vec2 ReadUV(const FbxMesh* inMesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer);


		
	};


}