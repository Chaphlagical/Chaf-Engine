#include <Scene/model.h>
#include <Renderer/command.h>
#include <iostream>

namespace Chaf
{
	void TriMesh::Create(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
	{
		m_Type = MeshType::Model;
		m_Vertices = vertices;
		m_Indices = indices;
		for (size_t i = 0; i < m_Indices.size(); i += 3)
		{
			Triangle triangle = { m_Indices[i],m_Indices[i + 1ul],m_Indices[i + 2ul] };
			m_Triangle.push_back(triangle);
		}
		m_RenderData = CreateScope<RenderData>();
		InitMesh();
	}

	void TriMesh::Create(MeshType type, int sample)
	{
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

		m_RenderData = CreateScope<RenderData>();
		InitMesh();
	}

	void TriMesh::InitMesh()
	{
		m_RenderData->m_VertexArray = VertexArray::Create();

		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer = VertexBuffer::Create((float*)(&*m_Vertices.begin()), sizeof(Vertex) * m_Vertices.size());

		BufferLayout layout =
		{
			{ShaderDataType::Float3,"a_Position"},
			{ShaderDataType::Float2,"a_TexCoord"},
			{ShaderDataType::Float3,"a_Normal"}			
		};

		vertexBuffer->SetLayout(layout);
		m_RenderData->m_VertexArray->AddVertexBuffer(vertexBuffer);

		Ref<IndexBuffer> indexBuffer;
		indexBuffer = IndexBuffer::Create((uint32_t*)(&*m_Indices.begin()), m_Indices.size());
		m_RenderData->m_VertexArray->AddIndexBuffer(indexBuffer);

		m_RenderData->m_Texture = Texture2D::Create(1, 1);
		m_RenderData->m_DefaultTexture = Texture2D::Create(1, 1);
		uint32_t defaultTextureData = 0xffffffff;
		m_RenderData->m_Texture->SetData(&defaultTextureData, sizeof(uint32_t));
		m_RenderData->m_DefaultTexture->SetData(&defaultTextureData, sizeof(uint32_t));

		m_RenderData->m_Shader = Shader::Create("assets/shader/texture.glsl");
		m_RenderData->m_Shader->Bind();
		m_RenderData->m_Shader->SetInt("u_Texture", 0);
	}

	void TriMesh::SetTexture(const std::string path)
	{
		m_RenderData->m_Texture = Texture2D::Create(path);
	}

	void TriMesh::ResetTexture()
	{
		m_RenderData->m_Texture = Texture2D::Create(1, 1);
		uint32_t defaultTextureData = 0xffffffff;
		m_RenderData->m_Texture->SetData(&defaultTextureData, sizeof(uint32_t));
	}

	void TriMesh::SetShader(const std::string path)
	{
		m_RenderData->m_Shader = Shader::Create(path);
		m_RenderData->m_Shader->Bind();
		m_RenderData->m_Shader->SetInt("u_Texture", 0);
	}

	void TriMesh::Draw(const Camera& camera, const bool lineMode)
	{
		m_RenderData->m_Shader->Bind();
		m_RenderData->m_Shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		m_RenderData->m_Shader->SetMat4("u_Transform", m_Posture.m_Transform);
		m_RenderData->m_Shader->SetFloat4("u_Color", m_Color);
		m_RenderData->m_Texture->Bind();
		RenderCommand::SetLineMode(lineMode);
		if (lineMode)m_RenderData->m_DefaultTexture->Bind();
		m_RenderData->m_VertexArray->Bind();
		RenderCommand::DrawIndexed(m_RenderData->m_VertexArray);
	}

	void TriMesh::CreatePlane(int sample)
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
				if ((k + 1)%(sample+1)>0)
					m_Indices.push_back(indices[i] + k);
		for (size_t i = 0; i < m_Indices.size(); i += 3)
		{
			Triangle triangle = { m_Indices[i],m_Indices[i + 1ul],m_Indices[i + 2ul] };
			m_Triangle.push_back(triangle);
		}		
	}

	void TriMesh::CreateCube(int sample)
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
					case 0:
						v.m_Position = { (float)i / (float)sample - 0.5f, 0.5f, (float)j / (float)sample - 0.5f };
						v.m_Normal = { 0.0f, 1.0f, 0.0f };
						m_Vertices.push_back(v);
						break;
					case 1:
						v.m_Position = { (float)i / (float)sample - 0.5f, -0.5f, (float)j / (float)sample - 0.5f };
						v.m_Normal = { 0.0f, -1.0f, 0.0f };
						m_Vertices.push_back(v);
						break;
					case 2:
						v.m_Position = { 0.5f, (float)i / (float)sample - 0.5f, (float)j / (float)sample - 0.5f };
						v.m_Normal = { 1.0f, 0.0f, 0.0f };
						m_Vertices.push_back(v);
						break;
					case 3:
						v.m_Position = { -0.5f, (float)i / (float)sample - 0.5f, (float)j / (float)sample - 0.5f };
						v.m_Normal = { -1.0f, 0.0f, 0.0f };
						m_Vertices.push_back(v);
						break;
					case 4:
						v.m_Position = { (float)i / (float)sample - 0.5f, (float)j / (float)sample - 0.5f, 0.5f };
						v.m_Normal = { 0.0f, 0.0f, 1.0f };
						m_Vertices.push_back(v);
						break;
					case 5:
						v.m_Position = { (float)i / (float)sample - 0.5f, (float)j / (float)sample - 0.5f, -0.5f };
						v.m_Normal = { 0.0f, 0.0f, -1.0f };
						m_Vertices.push_back(v);
						break;
					default:
						break;
					}
					
				}
		uint32_t indices_1[6] = { 0, sample + 1, sample + 2, 0, sample + 2,1 };
		uint32_t indices_2[6] = { 0, sample + 1, 1, sample + 1, 1, sample + 2 };
		for (int k = 0; k < 6; k++)
			for (int j = 0; j < (sample + 1) * sample; j++)
				for (int i = 0; i < 6; i++)
					if ((j + 1) % (sample + 1) > 0)
						if(k%2)
							m_Indices.push_back(indices_1[i] + j + k * (sample + 1) * (sample + 1));
						else
							m_Indices.push_back(indices_2[i] + j + k * (sample + 1) * (sample + 1));
		for (size_t i = 0; i < m_Indices.size(); i += 3)
		{
			Triangle triangle = { m_Indices[i],m_Indices[i + 1ul],m_Indices[i + 2ul] };
			m_Triangle.push_back(triangle);
		}		
	}

	void TriMesh::CreateSphere(int sample)
	{
		m_Type = MeshType::Sphere;
		for (int i = 0; i <= sample; i++)
			for (int j = 0; j <= sample; j++)
			{
				Vertex v;
				v.m_Position = { (float)i / (float)sample, 0.0f, (float)j / (float)sample };
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
		for (auto &p : m_Vertices)
		{
			float phi = glm::radians(360.0f * p.m_Position.z);
			float theta = glm::radians(180.0f * p.m_Position.x - 90.0f);
			p.m_Position.x = p.m_Normal.x = cos(theta) * cos(phi);
			p.m_Position.y = p.m_Normal.y = sin(theta);
			p.m_Position.z = p.m_Normal.z = cos(theta) * sin(phi);
		}
	}
}