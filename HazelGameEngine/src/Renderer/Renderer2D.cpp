#include "hzpch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Hazel
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		/*Square*/
		s_Data->QuadVertexArray = VertexArray::Create();

		// 5 floats per each vertex
		/*Vertex Pos + Texture Cordinate*/

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  /*Bottom Left  */
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  /*Bottom Right*/
			0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   /*Top Right*/
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f    /*Top Left*/
		};


		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		BufferLayout squareVBLayout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
		};

		squareVB->SetLayout(squareVBLayout);
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIdxB;
		squareIdxB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIdxB);

		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4(
			"u_ViewProjection", const_cast<OrthographicCamera&>(camera).GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ pos.x, pos.y, 0.f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color)
	{
		// 혹시나 문제 생기면, 여기에 Shader 한번 더 bind
		s_Data->TextureShader->SetFloat4("u_Color", color);

		// Bind Default White Texture


		// x,y 축 기준으로만 scale 을 조정할 것이다.
		glm::mat4 scale = glm::scale(glm::mat4(1.f), {size.x, size.y, 1.0f});
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * 
			/*rotation*/ scale;

		s_Data->TextureShader->SetMat4("u_Transform", transform);

		// actual draw call
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ pos.x, pos.y, 0.f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		s_Data->TextureShader->Bind();
		
		// 기본 Color 로 세팅
		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
	
		// default : 0번째 slot 에 세팅
		texture->Bind();

		// x,y 축 기준으로만 scale 을 조정할 것이다.
		glm::mat4 scale = glm::scale(glm::mat4(1.f), { size.x, size.y, 1.0f });
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) *
			/*rotation*/ scale;

		s_Data->TextureShader->SetMat4("u_Transform", transform);

		// actual draw call
		s_Data->QuadVertexArray->Bind();

		// 해당 함수안에 Texture Bind 가 존재한다.
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}

