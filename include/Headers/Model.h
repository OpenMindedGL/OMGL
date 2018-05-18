#pragma once

#include <string>
#include <vector>
#include "Vertex.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

template <class T>
class Model
{

  private : 

    std::vector<T>& m_Positions; 
    std::vector<unsigned int>& m_Indices; 
    unsigned int m_RendererType; 

    VertexArray * m_Va;
    VertexBuffer * m_Vb;
    VertexBufferLayout m_Layout;
    IndexBuffer * m_Ib;

    Shader * m_Shader;
    Texture m_Texture;

    glm::mat4 m_ModelMatrix;
    glm::mat4 m_Position; 
    glm::mat4 m_Rotation;
    

    void Push();
  public: 

    Model();
    Model(unsigned int renderType, std::vector<T>& positions, std::vector<unsigned int>& indices, std::string shaderPath);

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
    inline glm::mat4 GetModel() const { return m_ModelMatrix; }
};

template <class T>
void Model<T>::Push()
{
  m_Layout.Push<float>(3);
}

template <class T>
Model<T>::Model(unsigned int renderType, std::vector<T>& positions, std::vector<unsigned int>& indices, std::string shaderPath)
	: m_RendererType(renderType),  m_Positions(positions),
	  m_Indices(indices), m_ModelMatrix(glm::mat4(1.0f))
{

	Push();

	m_Va = new VertexArray();
	m_Vb = new VertexBuffer(&positions, positions.size() *  sizeof(T));
	m_Shader = new Shader(shaderPath);

	m_Va->AddBuffer(*m_Vb, m_Layout);

	m_Ib = new IndexBuffer((const unsigned int *) &indices, indices.size());

	m_Va->Unbind();
	m_Vb->Unbind();
	m_Ib->Unbind();
	m_Shader->Unbind();
}

