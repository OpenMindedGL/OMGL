#pragma once

#include <string>
#include <vector>
#include "Vertex.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"

template <class T>
struct is_vertex
{
	static const bool value = false;
};
template <>
struct is_vertex<Vertex>
{
	static const bool value = true;
};

template <class T>
struct is_vertexu
{
	static const bool value = false;
};
template <>
struct is_vertexu<Vertexu>
{
	static const bool value = true;
};

template <class T>
struct is_vertexun
{
	static const bool value = false;
};
template <>
struct is_vertexun<Vertexun>
{
	static const bool value = true;
};

template <class T>
class Model
{

private:

	std::vector<T> * m_Vertex;
	std::vector<unsigned int> * m_Indices;
	
	unsigned int m_RendererType;

	VertexArray * m_Va;
	VertexBuffer * m_Vb;
	VertexBufferLayout m_Layout;
	IndexBuffer * m_Ib;

	Shader * m_Shader;
	Texture m_Texture;

	glm::mat4 m_ModelMatrix;

	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;

	void Push();

public:

	Model();
	Model(unsigned int renderType, std::vector<T> * positions, std::vector<unsigned int> * indices, std::string shaderPath);

	//void initTexture(const std::string name, unsigned int id);
	void Bind();
	void Unbind();

	void Init();

	//SETTER
	void SetShader(const std::string path);
	void SetTexture(const std::string path);
	void SetShaderUniformMat4f(const std::string name, glm::mat4 mvp);

	void Translate(glm::vec3 position);
	void Rotation(float angle, glm::vec3 axis);
	void Scale(glm::vec3 scale);

	// GETTERS
	inline VertexArray & GetVertexArray() const { return *m_Va; }
	inline IndexBuffer & GetIndexBuffer() const { return *m_Ib; }
	inline Shader & GetShader() const { return *m_Shader; }
	inline unsigned int GetRendererType() const { return m_RendererType; }
	inline glm::mat4 GetModel() const { return m_ModelMatrix; }
};

template <class T>
Model<T>::Model() //:
{
	//m_Vertex = new std::vector;
}


template<class T>
Model<T>::Model(unsigned int renderType, std::vector<T> * positions, std::vector<unsigned int> * indices, std::string shaderPath)
	: m_RendererType(renderType), m_Vertex(positions),
	  m_Indices(indices), m_ModelMatrix(glm::mat4(1.0f)),
	  m_Position(glm::vec3(0.0f, 0.0f, 0.0f)), m_Rotation(glm::vec3(0.0f, 0.0f, 0.0f)), 
	  m_Scale(glm::vec3(1.0f))
{

	//Push();

	//m_Va = new VertexArray();
	//m_Vb = new VertexBuffer(positions, positions->size() * sizeof(T));
	m_Shader = new Shader(shaderPath);

	//m_Va->AddBuffer(*m_Vb, m_Layout);

	//m_Ib = new IndexBuffer((const unsigned int *)indices, indices->size());

	//m_Va->Unbind();
	//m_Vb->Unbind();
	//m_Ib->Unbind();
	m_Shader->Unbind();
};

template <class T>
void Model<T>::Bind()
{
	m_Va->Bind();
	m_Ib->Bind();
	m_Shader->Bind();
};

template <class T>
void Model<T>::Unbind()
{
	m_Va->Unbind();
	m_Ib->Unbind();
	m_Shader->Unbind();
};

template <class T>
void Model<T>::Init() 
{
	Push();

	m_Va = new VertexArray();
	m_Vb = new VertexBuffer(m_Vertex, m_Vertex->size() * sizeof(T));

	m_Va->AddBuffer(*m_Vb, m_Layout);

	m_Ib = new IndexBuffer((const unsigned int *)m_Indices, m_Indices->size());

	m_Va->Unbind();
	m_Vb->Unbind();
	m_Ib->Unbind();
}

template <class T>
void Model<T>::SetShader(const std::string path)
{
	m_Shader = new Shader(path);
	m_Shader->Unbind();
};

template <class T>
void Model<T>::SetTexture(const std::string path)
{
	Texture texture(path);
};

template <class T>
void Model<T>::SetShaderUniformMat4f(const std::string name, glm::mat4 mvp)
{
	m_Shader->SetUniformMat4f(name, mvp);
};

template <class T>
void Model<T>::Translate(glm::vec3 position)
{
	m_ModelMatrix = glm::translate(m_ModelMatrix, position);
	m_Position += position;
};

template <class T>
void Model<T>::Rotation(float angle, glm::vec3 axis)
{
	m_ModelMatrix = glm::rotate(m_ModelMatrix, angle, axis);
	m_Rotation.x += axis.x * angle;
	m_Rotation.y += axis.y * angle;
	m_Rotation.z += axis.z * angle;
};

template <class T>
void Model<T>::Scale(glm::vec3 scale)
{
	m_ModelMatrix = glm::scale(m_ModelMatrix, scale);
	m_Scale *= scale;
};

template <class T>
void Model<T>::Push()
{
	 if (is_vertexun<T>::value) {
		 m_Layout.Push<float>(3);
		 m_Layout.Push<float>(2);
		 m_Layout.Push<float>(3);
	}
	else if (is_vertexu<T>::value) {
		m_Layout.Push<float>(3);
		m_Layout.Push<float>(2);
	}
	else {
		m_Layout.Push<float>(3);
	}
};

/*
template <>
void Model<Vertex>::Push()
{
	m_Layout.Push<float>(3);
};

template <>
void Model<Vertexu>::Push()
{
	m_Layout.Push<float>(3);
	m_Layout.Push<float>(2);
};

template <>
void Model<Vertexun>::Push()
{
	m_Layout.Push<float>(3);
	m_Layout.Push<float>(2);
	m_Layout.Push<float>(3);
}*/

/*void Model<T>::initTexture(const std::string name, unsigned int id)
{
m_Shader.Bind();
m_Texture.Bind(id);
m_Shader.setUniform1i(name, id);
m_Texture.Unbind();
m_Shader.Unbind();
}*/

