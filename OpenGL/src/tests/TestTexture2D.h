#pragma once

#include "Test.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>
#include <array>

struct Vec2
{
	float x, y;
};

struct Vec3
{
	float x, y, z;
};

struct Vec4
{
	float x, y, z, w;
};

struct Vertex
{
	Vec3 Position;
	Vec4 Color;
	Vec2 TexCoord;
	float TexID;
};

namespace test
{
	class TestTexture2D: public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float deltatime) override;
		void OnRender() override;
		void OnIMGuiRender() override;
		static GLuint LoadTexture(const std::string path);
		static Vertex* CreateQuad(Vertex* target, float x, float y, float textureID);
	private:
		unsigned int m_RendererID;

		std::unique_ptr<VertexArray> m_Vao;
		std::unique_ptr<VertexBuffer> m_Vb;
		std::unique_ptr<IndexBuffer> m_Ib;
		//std::unique_ptr<Texture> m_Texture1;
		//std::unique_ptr<Texture> m_Texture2;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBufferLayout> m_Layout;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_TranslationA, m_TranslationB;

		GLuint m_Texture1, m_Texture2, m_Texture3, m_Texture4;

		float m_QuadPositionA[2] = { 0.75f, 0.75f };
		float m_Rotation[3] = {0.0f, 0.0f, 0.0f};
	};
}
