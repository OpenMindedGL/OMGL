#include "Model.h"
#include "Vertex.h"
#include <vector>



template <class T>
void Model<T>::Bind()
{
	m_Va->Bind();
	//m_Vb->Bind();
	m_Ib->Bind();
	m_Shader->Bind();
}

template <class T>
void Model<T>::Unbind()
{
	m_Va->Unbind();
	//m_Vb->Unbind();
	m_Ib->Unbind();
	m_Shader->Unbind();
}

template <class T>
void Model<T>::SetShader(const std::string path)
{
	m_Shader = new Shader(path);
	m_Shader->Unbind();
}

template <class T>
void Model<T>::SetTexture(const std::string path)
{
	Texture texture(path);
}

template <class T>
void Model<T>::SetShaderUniformMat4f(const std::string name, glm::mat4 mvp)
{
	m_Shader->SetUniformMat4f(name, mvp);
}

template <class T>
void Model<T>::Translate(glm::vec3 position)
{
	m_ModelMatrix = glm::translate(m_ModelMatrix, position);
}

template <class T>
void Model<T>::Rotation(float angle, glm::vec3 axis)
{
	m_ModelMatrix = glm::rotate(m_ModelMatrix, angle, axis);
}

template <class T>
void Model<T>::Scale(glm::vec3 scale)
{
	m_ModelMatrix = glm::scale(m_ModelMatrix, scale);
}

template <>
void Model<Vertex>::Push()
{
  m_Layout.Push<float>(3);
}

template <>
void Model<Vertexu>::Push()
{
  m_Layout.Push<float>(3);
  m_Layout.Push<float>(2);
}

template <>
void Model<Vertexun>::Push()
{
  m_Layout.Push<float>(3);
  m_Layout.Push<float>(2);
  m_Layout.Push<float>(3);
}
/*void Model<T>::initTexture(const std::string name, unsigned int id)
{
	m_Shader.Bind();
	m_Texture.Bind(id);
	m_Shader.setUniform1i(name, id);
	m_Texture.Unbind();
	m_Shader.Unbind();
}*/
