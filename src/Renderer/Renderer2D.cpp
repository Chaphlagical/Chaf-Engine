#include <Renderer/vao.h>
#include <Renderer/shader.h>
#include <Renderer/texture.h>
#include <Renderer/Renderer2D.h>
#include <Renderer/command.h>

namespace Chaf
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> DefaultTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		s_Data->QuadVertexArray = Chaf::VertexArray::Create();

		/*float vertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,0.0f,0.0f,
			0.5f, -0.5f, 0.0f,1.0f,0.0f,
			0.5f, 0.5f, 0.0f,1.0f,1.0f,
			-0.5f, 0.5f, 0.0f,0.0f,1.0f
		};

		Chaf::Ref<Chaf::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer = Chaf::VertexBuffer::Create(vertices, sizeof(vertices));
		CHAF_INFO(sizeof(vertices));*/

		std::vector<float> vertices = { 
			-0.5f, -0.5f, 0.0f,0.0f,0.0f,
			0.5f, -0.5f, 0.0f,1.0f,0.0f,
			0.5f, 0.5f, 0.0f,1.0f,1.0f,
			-0.5f, 0.5f, 0.0f,0.0f,1.0f };

		Chaf::Ref<Chaf::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer = Chaf::VertexBuffer::Create(&*vertices.begin(), sizeof(float)*vertices.size());

		Chaf::BufferLayout layout = {
			{Chaf::ShaderDataType::Float3,"a_Position"},
			{Chaf::ShaderDataType::Float2,"a_TexCoord"}
		};

		m_VertexBuffer->SetLayout(layout);
		s_Data->QuadVertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[6] = { 0,1,2, 2, 3, 0 };
		Chaf::Ref<Chaf::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer=Chaf::IndexBuffer::Create(indices, 6);
		s_Data->QuadVertexArray->AddIndexBuffer(m_IndexBuffer);

		s_Data->DefaultTexture = Texture2D::Create(1, 1);
		uint32_t defaultTextureData = 0xffffffff;
		s_Data->DefaultTexture->SetData(&defaultTextureData, sizeof(uint32_t));

		s_Data->TextureShader = Chaf::Shader::Create("assets/shader/texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		
	}

	void Renderer2D::SetLineMode(const bool enable)
	{
		Chaf::RenderCommand::SetLineMode(enable);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		s_Data->TextureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat4("u_Color", color);

		texture->Bind();

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		

		s_Data->TextureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));

		texture->Bind();

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->TextureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat4("u_Color", color);

		s_Data->DefaultTexture->Bind();

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}