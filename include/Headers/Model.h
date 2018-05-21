#ifndef Model_H
#define Model_H


#include <string>
#include <vector>
#include "Texture.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"

template <class T>
class Model
{

  private : 

    unsigned int m_RendererType; 

    VertexArray * m_Va;
    VertexBufferLayout m_Layout;
    IndexBuffer * m_Ib;

    Shader * m_Shader;
    Texture m_Texture;

    glm::mat4 m_ModelMatrix;
    glm::mat4 m_Position; 
    glm::mat4 m_Rotation;
    

    void Push();
  public: 

    VertexBuffer * m_Vb;
    std::vector<unsigned int> m_Indices; 
    std::vector<T> m_Positions; 
    unsigned int m_nbIndices;
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
    void Upload();
    void Init(unsigned int renderType, std::string shaderPat);
    void ComputeNormals();



    // GETTERS
    inline VertexArray & GetVertexArray() const { return *m_Va; }
    inline IndexBuffer & GetIndexBuffer() const { return *m_Ib; }
    inline Shader & GetShader() const { return *m_Shader; }
    inline unsigned int GetRendererType() const { return m_RendererType; }
    inline glm::mat4 GetModelMatrix() const { return m_ModelMatrix; }
};

template <class T>
void Model<T>::ComputeNormals(){
  
  unsigned int i1 = m_Indices[0];
  unsigned int i2 = m_Indices[1];
  unsigned int i3;
  glm::vec3 triangle_normal;
  bool order = true;
  for(unsigned int i=2;i<m_Indices.size();i++){
    if(m_Indices[i] == m_Positions.size()){
      // reached end of line (index == restart index)
      i1 = m_Indices[++i];
      i2 = m_Indices[++i];
      i++;
      order = true;
    }
    i3 = m_Indices[i];
    // compute cross product in right order
    if(order)
      triangle_normal = normalize(cross(m_Positions[i2].pos-m_Positions[i1].pos,m_Positions[i3].pos-m_Positions[i1].pos));
    else
      triangle_normal = normalize(cross(m_Positions[i2].pos-m_Positions[i3].pos,m_Positions[i1].pos-m_Positions[i3].pos));
    m_Positions[i1].normal += triangle_normal;
    m_Positions[i2].normal += triangle_normal;
    m_Positions[i3].normal += triangle_normal;
    i1 = i2;
    i2 = i3;
    order = !order;
  }
  for(unsigned int i = 0; i<m_Positions.size();i++){
    m_Positions[i].normal = normalize(m_Positions[i].normal);
  } 

}


template <class T>
void Model<T>::Init(unsigned int renderType, std::string shaderPath)
{
  m_RendererType = renderType;
  m_Layout.Push<float>(3);
  m_Layout.Push<float>(2);
  m_Layout.Push<float>(3);

  m_Va = new VertexArray();
  m_Vb = new VertexBuffer((const void *) &m_Positions[0], m_Positions.size() *  sizeof(T));
  m_Shader = new Shader(shaderPath);

  m_Va->AddBuffer(*m_Vb, m_Layout);

  GLCall(glEnable(GL_PRIMITIVE_RESTART));
  GLCall(glPrimitiveRestartIndex(m_Positions.size()));
  m_Ib = new IndexBuffer((const unsigned int *) &m_Indices[0], m_Indices.size());

  //if(renderType == GL_TRIANGLE_STRIP)

  m_Va->Unbind();
  m_Vb->Unbind();
  m_Ib->Unbind();
  m_Shader->Unbind();
}

template <class T>
void Model<T>::Push()
{
  m_Layout.Push<float>(3);
}

template <class T>
Model<T>::Model()
{
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

	m_Ib = new IndexBuffer((const unsigned int *) &indices[0], indices.size());

	m_Va->Unbind();
	m_Vb->Unbind();
	m_Ib->Unbind();
	m_Shader->Unbind();
}

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

template <class T>
void Model<T>::Upload(){
        (*m_Vb).Upload();
}
/*void Model<T>::initTexture(const std::string name, unsigned int id)
{
	m_Shader.Bind();
	m_Texture.Bind(id);
	m_Shader.setUniform1i(name, id);
	m_Texture.Unbind();
	m_Shader.Unbind();
}*/

#endif
