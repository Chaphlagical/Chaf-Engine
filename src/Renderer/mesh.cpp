#include <Renderer/mesh.h>
#include <Renderer/command.h>
#include <Renderer/tinyobjloader/tiny_obj_loader.h>

namespace Chaf
{
	TriMesh::TriMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		m_Type = MeshType::Model;
		m_Vertices = vertices;
		m_Indices = indices;
		for (size_t i = 0; i < m_Indices.size(); i += 3)
		{
			Triangle triangle = { m_Indices[i],m_Indices[i + 1ul],m_Indices[i + 2ul] };
			m_Triangle.push_back(triangle);
		}
		GenTBN();
		GenVertexArray();
	}

	TriMesh::TriMesh(const MeshType& type, const uint32_t& sample)
		:m_Type(type)
	{
		m_HasNormal = true;
		m_HasTexCoord = true;
		switch (type)
		{
		case Chaf::MeshType::None:
			CHAF_CORE_ASSERT(false, "MeshType::None is unsupported mesh type!");
			return;
		case Chaf::MeshType::Plane:
			CreatePlane(sample);
			break;
		case Chaf::MeshType::Cube:
			CreateCube(sample);
			break;
		case Chaf::MeshType::Sphere:
			CreateSphere(sample);
			break;
		case Chaf::MeshType::Model:
			CHAF_CORE_ASSERT(false, "MeshType::Model can't init by this method!");
			return;
		default:
			break;
		}
		GenTBN();
		GenVertexArray();
	}

	TriMesh::TriMesh(const std::string& path)
	{
		m_Type = MeshType::Model;
		ObjLoader(path);
		GenTBN();
		GenVertexArray();
	}

	void TriMesh::GenTBN()
	{
		if (!m_HasNormal)
		{
			for (auto triangle : m_Triangle)
			{
				glm::vec3 v1 = m_Vertices[triangle.idx1].m_Position - m_Vertices[triangle.idx2].m_Position;
				glm::vec3 v2 = m_Vertices[triangle.idx2].m_Position - m_Vertices[triangle.idx3].m_Position;
				glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
				m_Vertices[triangle.idx1].m_Normal = glm::normalize(m_Vertices[triangle.idx1].m_Normal + normal);
				m_Vertices[triangle.idx2].m_Normal = glm::normalize(m_Vertices[triangle.idx2].m_Normal + normal);
				m_Vertices[triangle.idx3].m_Normal = glm::normalize(m_Vertices[triangle.idx3].m_Normal + normal);
			}
		}
		for (auto& triangle : m_Triangle)
		{
			glm::vec3 norm = (m_Vertices[triangle.idx1].m_Normal + m_Vertices[triangle.idx2].m_Normal + m_Vertices[triangle.idx3].m_Normal) * glm::vec3(1.0f / 3.0f);
			
			glm::vec3 edge1 = m_Vertices[triangle.idx2].m_Position - m_Vertices[triangle.idx1].m_Position;
			glm::vec3 edge2 = m_Vertices[triangle.idx1].m_Position - m_Vertices[triangle.idx3].m_Position;
			glm::vec2 deltaUV1 = m_Vertices[triangle.idx2].m_TexCoord - m_Vertices[triangle.idx1].m_TexCoord;
			glm::vec2 deltaUV2 = m_Vertices[triangle.idx1].m_TexCoord - m_Vertices[triangle.idx3].m_TexCoord;

			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			glm::vec3 tangent;
			tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
			tangent = glm::normalize(tangent);

			glm::vec3 bitangent;
			bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
			bitangent = glm::normalize(bitangent);

			m_Vertices[triangle.idx1].m_Tangent = glm::normalize(m_Vertices[triangle.idx1].m_Tangent + tangent);
			m_Vertices[triangle.idx2].m_Tangent = glm::normalize(m_Vertices[triangle.idx2].m_Tangent + tangent);
			m_Vertices[triangle.idx3].m_Tangent = glm::normalize(m_Vertices[triangle.idx3].m_Tangent + tangent);

			m_Vertices[triangle.idx1].m_Bitangent = glm::normalize(m_Vertices[triangle.idx1].m_Bitangent + bitangent);
			m_Vertices[triangle.idx2].m_Bitangent = glm::normalize(m_Vertices[triangle.idx2].m_Bitangent + bitangent);
			m_Vertices[triangle.idx3].m_Bitangent = glm::normalize(m_Vertices[triangle.idx3].m_Bitangent + bitangent);
		}
	}

	void TriMesh::GenVertexArray()
	{
		m_VertexArray = VertexArray::Create();

		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer = VertexBuffer::Create((float*)(&*m_Vertices.begin()), sizeof(Vertex) * m_Vertices.size());

		BufferLayout layout =
		{
			{ShaderDataType::Float3,"a_Position"},
			{ShaderDataType::Float2,"a_TexCoord"},
			{ShaderDataType::Float3,"a_Normal"},
			{ShaderDataType::Float3,"a_Tangent"},
			{ShaderDataType::Float3,"a_Bitangent"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		Ref<IndexBuffer> indexBuffer;
		indexBuffer = IndexBuffer::Create((uint32_t*)(&*m_Indices.begin()), m_Indices.size());
		m_VertexArray->AddIndexBuffer(indexBuffer);
	}

	void TriMesh::Draw(bool lineMode)
	{
		if (m_Type == MeshType::None)return;
		RenderCommand::SetLineMode(lineMode);
		m_VertexArray->Bind();
		RenderCommand::DrawIndexed(m_VertexArray);
	}

	/////////////////////	Create or Load Mesh /////////////////////
	void TriMesh::CreatePlane(const uint32_t& sample)
	{
		m_Type = MeshType::Plane;
		for (int i = 0; i <= sample; i++)
			for (int j = 0; j <= sample; j++)
			{
				Vertex v;
				v.m_Position = { (float)i / (float)sample - 0.5f, 0.0f, (float)j / (float)sample - 0.5f };
				v.m_Normal = { 0.0f, 1.0f, 0.0f };
				v.m_TexCoord = { (float)i / (float)sample, (float)j / (float)sample };
				m_Vertices.push_back(v);
			}
		uint32_t indices[6] = { 0, sample + 1, sample + 2, 0, sample + 2,1 };
		for (int k = 0; k < (sample + 1) * sample; k++)
			for (int i = 0; i < 6; i++)
				if ((k + 1) % (sample + 1) > 0)
					m_Indices.push_back(indices[i] + k);
		for (size_t i = 0; i < m_Indices.size(); i += 3)
		{
			Triangle triangle = { m_Indices[i],m_Indices[i + 1ul],m_Indices[i + 2ul] };
			m_Triangle.push_back(triangle);
		}
	}

	void TriMesh::CreateCube(const uint32_t& sample)
	{
		m_Type = MeshType::Cube;
		for (int k = 0; k < 6; k++)
			for (int i = 0; i <= sample; i++)
				for (int j = 0; j <= sample; j++)
				{
					Vertex v;
					v.m_TexCoord = { (float)i / (float)sample, (float)j / (float)sample };
					switch (k)
					{
					case 0:	//	back face
						v.m_Position = { (float)i / (float)sample - 0.5f, (float)j / (float)sample - 0.5f, -0.5f };
						v.m_Normal = { 0.0f, 0.0f, -1.0f };
						m_Vertices.push_back(v);
						break;
					case 1:	//	front face
						v.m_Position = { (float)i / (float)sample - 0.5f, (float)j / (float)sample - 0.5f, 0.5f };
						v.m_Normal = { 0.0f, 0.0f, 1.0f };
						m_Vertices.push_back(v);
						break;
					case 2:	//	left face
						v.m_Position = { -0.5f, (float)i / (float)sample - 0.5f, (float)j / (float)sample - 0.5f };
						v.m_Normal = { -1.0f, 0.0f, 0.0f };
						m_Vertices.push_back(v);
						break;
					case 3:	//	right face
						v.m_Position = { 0.5f, (float)i / (float)sample - 0.5f, (float)j / (float)sample - 0.5f };
						v.m_Normal = { 1.0f, 0.0f, 0.0f };
						m_Vertices.push_back(v);
						break;
					case 4:	//	bottom face
						v.m_Position = { (float)i / (float)sample - 0.5f, -0.5f, (float)j / (float)sample - 0.5f };
						v.m_Normal = { 0.0f, -1.0f, 0.0f };
						m_Vertices.push_back(v);
						break;
					case 5:	// top face
						v.m_Position = { (float)i / (float)sample - 0.5f, 0.5f, (float)j / (float)sample - 0.5f };
						v.m_Normal = { 0.0f, 1.0f, 0.0f };
						m_Vertices.push_back(v);
						break;
					default:
						break;
					}
				}
		uint32_t indices_1[6] = { 0, sample + 2, sample + 1, sample + 2, 0, 1 };
		uint32_t indices_2[6] = { 0, sample + 2, sample + 1, sample + 2, 1, 0 };
		for (int k = 0; k < 6; k++)
			for (int j = 0; j < (sample + 1) * sample; j++)
				for (int i = 0; i < 6; i++)
					if ((j + 1) % (sample + 1) > 0)
						if (k % 2)
							m_Indices.push_back(indices_1[i] + j + k * (sample + 1) * (sample + 1));
						else
							m_Indices.push_back(indices_2[i] + j + k * (sample + 1) * (sample + 1));
		for (size_t i = 0; i < m_Indices.size(); i += 3)
		{
			Triangle triangle = { m_Indices[i],m_Indices[i + 1ul],m_Indices[i + 2ul] };
			m_Triangle.push_back(triangle);
		}
	}

	void TriMesh::CreateSphere(const uint32_t& sample)
	{
		uint32_t real = sample * 10;
		m_Type = MeshType::Sphere;
		for (int i = 0; i <= real; i++)
			for (int j = 0; j <= real; j++)
			{
				Vertex v;
				v.m_Position = { (float)i / (float)real, 0.0f, (float)j / (float)real };
				v.m_Normal = { 0.0f, 1.0f, 0.0f };
				v.m_TexCoord = { (float)i / (float)real, (float)j / (float)real };
				m_Vertices.push_back(v);
			}
		uint32_t indices[6] = { 0, real + 1, real + 2, 0, real + 2,1 };
		for (int k = 0; k < (real + 1) * real; k++)
			for (int i = 0; i < 6; i++)
				if ((k + 1) % (real + 1) > 0)
					m_Indices.push_back(indices[i] + k);
		for (size_t i = 0; i < m_Indices.size(); i += 3)
		{
			Triangle triangle = { m_Indices[i],m_Indices[i + 1ul],m_Indices[i + 2ul] };
			m_Triangle.push_back(triangle);
		}
		for (auto& p : m_Vertices)
		{
			float phi = glm::radians(360.0f * p.m_Position.z);
			float theta = glm::radians(180.0f * p.m_Position.x - 90.0f);
			p.m_Position.x = p.m_Normal.x = cos(theta) * cos(phi);
			p.m_Position.y = p.m_Normal.y = sin(theta);
			p.m_Position.z = p.m_Normal.z = cos(theta) * sin(phi);
		}
	}

	void TriMesh::ObjLoader(const std::string& path)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str()))
		{
			CHAF_CORE_ERROR("Couldn't load .obj model!");
			CHAF_CORE_ASSERT(false, "Couldn't load .obj model!");
			m_Type = MeshType::None;
			return;
		}
		m_Path = static_cast<std::string>(path);
		std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
		if (attrib.normals.size() > 0)m_HasNormal = true; else m_HasNormal = false;
		if (attrib.texcoords.size() > 0)m_HasTexCoord = true; else m_HasTexCoord = false;
		for (auto const& shape : shapes)
			for (auto const& index : shape.mesh.indices)
			{
				Vertex v;
				if (attrib.vertices.size() > 3 * index.vertex_index + 2)
					v.m_Position = {
						attrib.vertices[3 * index.vertex_index + 0],
						attrib.vertices[3 * index.vertex_index + 1],
						attrib.vertices[3 * index.vertex_index + 2]
				};
				if (attrib.texcoords.size() > 0)
					v.m_TexCoord = {
						attrib.texcoords[2 * index.texcoord_index + 0],
						attrib.texcoords[2 * index.texcoord_index + 1]
				};
				else v.m_TexCoord = { 0.0f, 0.0f };
				if (attrib.normals.size() > 0)
					v.m_Normal = {
						attrib.normals[3 * index.normal_index + 0],
						attrib.normals[3 * index.normal_index + 1],
						attrib.normals[3 * index.normal_index + 2]
				};
				else v.m_Normal = { 0.0f, 0.0f, 0.0f };
				if (uniqueVertices.count(v) == 0)
				{
					uniqueVertices[v] = static_cast<uint32_t>(m_Vertices.size());
					m_Vertices.push_back(v);
				}
				m_Indices.push_back(uniqueVertices[v]);
			}
		for (size_t i = 0; i < m_Indices.size(); i += 3)
		{
			Triangle triangle = { m_Indices[i],m_Indices[i + 1ul],m_Indices[i + 2ul] };
			m_Triangle.push_back(triangle);
		}
	}
}