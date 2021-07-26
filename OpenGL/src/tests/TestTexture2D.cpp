#include "TestTexture2D.h"

#include "Renderer.h"

#include "imgui/imgui.h"
#include "stb-image/stb_image.h"

namespace test
{
	TestTexture2D::TestTexture2D()
		:m_Proj(glm::ortho(0.0f, 5.0f, 0.0f, 5.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_TranslationA(0, 0, 0), m_TranslationB(0, 0, 0)
	{


		/*unsigned int indicies[] = {
			0, 1, 2, 
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			8, 9, 10,
			10, 11, 8,

			12, 13, 14, 
			14, 15, 12
		};*/

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		const size_t MaxQuadCount = 1000;
		const size_t MaxVertexCount = MaxQuadCount * 4;
		const size_t MaxIndexCount = MaxQuadCount * 6;

		unsigned int indicies[MaxIndexCount];
		unsigned int offset = 0;
		for (size_t i =  0; i < MaxIndexCount; i += 6)
		{
			indicies[i + 0] = 0 + offset;
			indicies[i + 1] = 1 + offset;
			indicies[i + 2] = 2 + offset;
					 
			indicies[i + 3] = 2 + offset;
			indicies[i + 4] = 3 + offset;
			indicies[i + 5] = 0 + offset;

			offset += 4;
		}

		m_Vao = std::make_unique<VertexArray>();
		m_Vb = std::make_unique<VertexBuffer>(nullptr, MaxVertexCount *  sizeof(Vertex));
		m_Layout = std::make_unique<VertexBufferLayout>();
		m_Layout->Push<float>(3);
		m_Layout->Push<float>(4);
		m_Layout->Push<float>(2);
		m_Layout->Push<float>(1);

		m_Vao->AddBuffer(*m_Vb, *m_Layout);
		//m_Ib = std::make_unique<IndexBuffer>(indicies, 6000);

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

		m_Shader = std::make_unique<Shader>("res/shader/Basic.shader");
		m_Shader->Bind();
		//m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		//m_Texture1 = std::make_unique<Texture>("res/texture/ing.png");
		//m_Texture2 = std::make_unique<Texture>("res/texture/ing2.png");
		//m_Shader->SetUniform1i("u_Textures", 0);
		int samplers[3] = { 0, 1, 2 };
		m_Shader->SetUniform1iv("u_Textures", 3, samplers);

		m_Texture1 = LoadTexture("res/texture/ing2.png");
		m_Texture2 = LoadTexture("res/texture/ing4.png");
		m_Texture3 = LoadTexture("res/texture/ing3.png");
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltatime)
	{
	}

	void TestTexture2D::OnRender()
	{
		//set dynamic vertex buffer
		/*float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, 0.0f,//0
			 50.0f, -50.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f, 0.0f,//1
			 50.0f,  50.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 1.0f, 0.0f,//2
			-50.0f,  50.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 1.0f, 0.0f,//3

			100.0f, -50.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, 1.0f,//4
			200.0f, -50.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 0.0f, 1.0f,//5
			200.0f,  50.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 1.0f, 1.0f,//6
			100.0f,  50.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 1.0f, 1.0f,//7

			250.0f, -50.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f,//8
			350.0f, -50.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 2.0f,//9
			350.0f,  50.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f,//10
			250.0f,  50.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 2.0f //11
		};*/

		unsigned int indexCount = 0;

		std::array<Vertex, 1000> Position;
		Vertex* buffer = Position.data();
		for (int y = 0; y < 5; y++)
		{
			for (int x = 0; x < 5; x++)
			{
				buffer = CreateQuad(buffer, x, y, (x + y) % 2);
				indexCount += 6;
			}
		}

		buffer = CreateQuad(buffer, m_QuadPositionA[0], m_QuadPositionA[1], 2);
		indexCount += 6;

		m_Vb->Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, Position.size() * sizeof(Vertex), Position.data());

		GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		glBindTextureUnit(0, m_Texture1);
		glBindTextureUnit(1, m_Texture2);
		glBindTextureUnit(2, m_Texture3);

		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), m_Rotation[0], glm::vec3(0, 0, 1));
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model * rotation;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			//renderer.Draw(*m_Vao,*m_Ib, indexCount, *m_Shader);
			m_Vb->Bind();
			m_Shader->Bind();
			GLCall(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr));
		}
		//{
		//	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		//	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
		//	glm::mat4 mvp = m_Proj * m_View * model * rotation;
		//	m_Shader->SetUniformMat4f("u_MVP", mvp);
		//	m_Vb->Bind();
		//	m_Shader->Bind();
		//	renderer.Draw(*m_Vao, *m_Ib, 2, *m_Shader);
		//}
	}

	void TestTexture2D::OnIMGuiRender()
	{
		ImGui::DragFloat2("Quad Position A", m_QuadPositionA, 0.1f);
		ImGui::SliderAngle("Quad Rotation", m_Rotation, -360.0f, 360.0f);
		//ImGui::DragFloat2("Quad Position B", m_QuadPositionB, 1.0f);
		//ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
		//ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	GLuint TestTexture2D::LoadTexture(const std::string path)
	{
		int w, h, bits;

		stbi_set_flip_vertically_on_load(1);
		auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, 4);

		GLuint textureID;
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &textureID));
		GLCall(glBindTexture(GL_TEXTURE_2D, textureID));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));

		stbi_image_free(pixels);

		return textureID;
	}

	Vertex* TestTexture2D::CreateQuad(Vertex* target, float x, float y, float textureID)
	{
		float size = 1.0f;
		
		target->Position = { x, y, 0.0f };
		target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
		target->TexCoord = { 0.0f, 0.0f };
		target->TexID = textureID;
		target++;
		//
		
		target->Position = { x + size, y, 0.0f };
		target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
		target->TexCoord = { 1.0f, 0.0f };
		target->TexID = textureID;
		target++;
		//
		
		target->Position = { x + size, y + size, 0.0f };
		target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
		target->TexCoord = { 1.0f, 1.0f };
		target->TexID = textureID;
		target++;
		//
		
		target->Position = { x, y + size, 0.0f };
		target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
		target->TexCoord = { 0.0f, 1.0f };
		target->TexID = textureID;
		target++;

		return target;
	}

}