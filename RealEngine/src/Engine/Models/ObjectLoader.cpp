#include "repch.h"
#include "ObjectLoader.h"

namespace RealEngine {

	std::pair<std::vector<glm::vec3>, std::vector<uint32_t>> ObjectLoader::LoadObjectFromOBJ(const std::string& fileName)
	{
		//Vertex portions
		std::vector<glm::vec3> vertex_positions;
		std::vector<glm::vec2> vertex_texcoords;
		std::vector<glm::vec3> vertex_normals;

		//Face vectors
		std::vector<uint32_t> vertex_position_indicies;
		std::vector<uint32_t> vertex_texcoord_indicies;
		std::vector<uint32_t> vertex_normal_indicies;

		//Vertex array
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		std::stringstream ss;
		std::ifstream in(fileName);
		std::string line;
		std::string prefix;
		glm::vec3 temp_vec3;
		glm::vec2 temp_vec2;
		GLint temp_glint = 0;

		//File open error check
		if (!in.is_open())
		{
			ENGINE_ERROR("Could not open file {0}", fileName);
		}

		//Read one line at a time
		while (std::getline(in, line))
		{
			//Get the prefix of the line
			ss.clear();
			ss.str(line);
			ss >> prefix;
			int j = 0;

			if (prefix == "#")
			{

			}
			else if (prefix == "o")
			{

			}
			else if (prefix == "s")
			{

			}
			else if (prefix == "use_mtl")
			{

			}
			else if (prefix == "v") //Vertex position
			{
				ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
				vertex_positions.push_back(temp_vec3);
			}
			else if (prefix == "vt")
			{
				ss >> temp_vec2.x >> temp_vec2.y;
				vertex_texcoords.push_back(temp_vec2);
			}
			else if (prefix == "vn")
			{
				ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
				vertex_normals.push_back(temp_vec3);
			}
			else if (prefix == "f")
			{
				// todo:: get index buffer array from faces

				std::vector<uint32_t> indeices;
				int counter = 0;
				int vertexCounter = 0;
				while (ss >> temp_glint)
				{
					//Pushing indices into correct arrays
					if (counter == 0)
					{
						vertex_position_indicies.push_back(temp_glint);
						indeices.push_back(temp_glint);
					}
					else if (counter == 1)
						vertex_texcoord_indicies.push_back(temp_glint);
					else if (counter == 2)
						vertex_normal_indicies.push_back(temp_glint);

					//Handling characters
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

					//Reset the counter
					if (counter > 2)
					{
						counter = 0;
						vertexCounter++;
					}
				}
				int i = 0;
				//std::cout << indeices.size();
				while(i < indeices.size() - 2)
				{
					indices.push_back(indeices[0]);
					indices.push_back(indeices[++i]);
					indices.push_back(indeices[i + 1]);
					//ENGINE_INFO("{0} {1} {2}", indexBufferIndices[j - 2], indexBufferIndices[j - 1], indexBufferIndices[j]);
				}

			}
			else
			{

			}
		}

				
		//Build final vertex array (mesh)
		vertices.resize(vertex_position_indicies.size(), Vertex());

		//Load in all indices
		for (size_t i = 0; i < vertices.size(); ++i)
		{
			vertices[i].Position = vertex_positions[vertex_position_indicies[i] - 1];
			vertices[i].TexCoord = vertex_texcoords[vertex_texcoord_indicies[i] - 1];
			vertices[i].Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//DEBUG
		std::cout << "Nr of vertices: " << vertices.size() << "\n";

		//for (Vertex vertex : vertices)
		//{
		//	ENGINE_INFO("{0}, {1},", vertex.TexCoord.x, vertex.TexCoord.y);
		//}

		//Loaded success
		std::cout << "OBJ file loaded!" << "\n";
		return { vertex_positions, indices };
	}

}