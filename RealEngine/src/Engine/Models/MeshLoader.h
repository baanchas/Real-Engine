#pragma once

#include "OpenGL/VertexBuffer.h"

namespace RealEngine {
	namespace MeshLoader {

		class FBX
		{
		public:
			// FBX extension func
			static void LoadMesh(const std::string& path, Mesh& myMesh);

		private:
			// FBX extension func
			static bool Initialize(FbxMesh* mesh, Mesh& myMesh);
			static void LoadNode(FbxNode* node, Mesh& myMesh);
			static void ProcessControlPoints(const FbxMesh* mesh, std::vector<glm::vec3>& positions);
			static glm::vec3 ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter);
			static glm::vec2 ReadUV(const FbxMesh* inMesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer);
		};

		class OBJ
		{
		public:
			// OBJ extension func
			static void LoadMesh(const std::string& file_name, Mesh& mesh);
		};

		class REM
		{
		public:
			// REM : the engine's model extension
			static void SaveMesh(std::string path, Mesh& mesh);
			static void LoadMesh(std::string path, Mesh& mesh);
		};
	}
}

