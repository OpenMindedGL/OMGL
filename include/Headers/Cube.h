#pragma once

#include <string>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

class Cube
{

private : 

	float * m_Positions;
	unsigned int * m_Indices;
	unsigned int m_RendererType; 

	VertexArray * m_Va;
	VertexBuffer * m_Vb;
	VertexBufferLayout m_Layout;
	IndexBuffer * m_Ib;

	Shader * m_Shader;
	Texture m_Texture;
	
	glm::mat4 m_Model;
	glm::mat4 m_Position; 
	glm::mat4 m_Rotation;

public: 
	
	Cube();
	Cube(unsigned int m_RendererType, float * positions, unsigned int * indices, std::string shaderPath);
	
	//void initTexture(const std::string name, unsigned int id);
	void Bind();
	void Unbind();

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
	inline glm::mat4 GetModel() const { return m_Model; }
};