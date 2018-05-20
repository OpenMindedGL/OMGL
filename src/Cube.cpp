#include "Cube.h"

Cube::Cube() {}

Cube::Cube(unsigned int renderType, float * positions, unsigned int * indices, std::string shaderPath)
	: m_RendererType(renderType),  m_Positions(positions),
	  m_Indices(indices), m_Model(glm::mat4(1.0f))
{

	m_Layout.Push<float>(3);

	m_Va = new VertexArray();
	m_Vb = new VertexBuffer(positions, 3 * 8 * sizeof(float));
	m_Shader = new Shader(shaderPath);

	m_Va->AddBuffer(*m_Vb, m_Layout);

	m_Ib = new IndexBuffer(indices, 3 * 12);

	m_Va->Unbind();
	m_Vb->Unbind();
	m_Ib->Unbind();
	m_Shader->Unbind();
}

void Cube::Bind()
{
	m_Va->Bind();
	//m_Vb->Bind();
	m_Ib->Bind();
	m_Shader->Bind();
}

void Cube::Unbind()
{
	m_Va->Unbind();
	//m_Vb->Unbind();
	m_Ib->Unbind();
	m_Shader->Unbind();
}

void Cube::SetShader(const std::string path)
{
	m_Shader = new Shader(path);
	m_Shader->Unbind();
}

void Cube::SetTexture(const std::string path)
{
	Texture texture(path);
}

void Cube::SetShaderUniformMat4f(const std::string name, glm::mat4 mvp)
{
	m_Shader->SetUniformMat4f(name, mvp);
}

void Cube::Translate(glm::vec3 position)
{
	m_Model = glm::translate(m_Model, position);
}

void Cube::Rotation(float angle, glm::vec3 axis)
{
	m_Model = glm::rotate(m_Model, angle, axis);
}

void Cube::Scale(glm::vec3 scale)
{
	m_Model = glm::scale(m_Model, scale);
}
/*void Cube::initTexture(const std::string name, unsigned int id)
{
	m_Shader.Bind();
	m_Texture.Bind(id);
	m_Shader.setUniform1i(name, id);
	m_Texture.Unbind();
	m_Shader.Unbind();
}*/
