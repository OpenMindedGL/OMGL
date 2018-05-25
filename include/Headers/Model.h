#ifndef Model_H
#define Model_H


#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"
#include "VertexBufferLayout.h"

template <class T>
class Model
{

  protected : 

    std::vector<unsigned int> m_Indices; 
    std::vector<T> m_Vertices; 
    Texture * m_Texture;

  private : 

    unsigned int m_RendererType; 

    VertexBuffer * m_Vb;
    VertexArray * m_Va;
    VertexBufferLayout m_Layout;
    IndexBuffer * m_Ib;

    Shader * m_Shader;

    glm::mat4 m_ModelMatrix;
    glm::mat4 m_Position; 
    glm::mat4 m_Rotation;
    


    void Push();
  public: 

    Model();
    Model(unsigned int renderType, std::vector<T>& positions, std::vector<unsigned int>& indices, std::string shaderPath);

    void InitTexture(const std::string name, unsigned int id);
    void Bind();
    void Unbind();

    //SETTER
    void SetShader(const std::string path);
    void SetTexture(const std::string path, const std::string name, unsigned int slot=0);
    void SetTexture(const std::string path);
    void SetShaderUniformMat4f(const std::string name, glm::mat4 mvp);

    void Translate(glm::vec3 position);
    void Rotation(float angle, glm::vec3 axis);
    void Scale(glm::vec3 scale);
    void Upload();
    void Init(unsigned int renderType, std::string shaderPat);
    void ComputeNormals();
    inline void BindVertexBuffer(){ m_Vb->Bind(); };



    // GETTERS
    inline VertexArray & GetVertexArray() const { return *m_Va; }
    inline IndexBuffer & GetIndexBuffer() const { return *m_Ib; }
    inline VertexBuffer & GetVertexBuffer() const { return *m_Vb; }
    inline Shader & GetShader() const { return *m_Shader; }
    inline unsigned int GetRendererType() const { return m_RendererType; }
    inline glm::mat4 GetModelMatrix() const { return m_ModelMatrix; }
    inline std::vector<T> GetVertices() const { return m_Vertices; }
    inline std::vector<unsigned int> GetIndices() const { return m_Indices; }
    inline Texture* GetTexture() const { return m_Texture; }
};

template <class T>
void Model<T>::ComputeNormals(){
  
  unsigned int i1 = m_Indices[0];
  unsigned int i2 = m_Indices[1];
  unsigned int i3;
  glm::vec3 triangle_normal;
  bool order = true;
  for(unsigned int i=2;i<m_Indices.size()-1;i++){
    if(m_Indices[i] == m_Vertices.size()){
      // reached end of line (index == restart index)
      i1 = m_Indices[++i];
      i2 = m_Indices[++i];
      i++;
      order = true;
    }
    i3 = m_Indices[i];
    // compute cross product in right order
    if(order)
      triangle_normal = normalize(cross(m_Vertices[i2].pos-m_Vertices[i1].pos,m_Vertices[i3].pos-m_Vertices[i1].pos));
    else
      triangle_normal = normalize(cross(m_Vertices[i2].pos-m_Vertices[i3].pos,m_Vertices[i1].pos-m_Vertices[i3].pos));
    m_Vertices[i1].normal += triangle_normal;
    m_Vertices[i2].normal += triangle_normal;
    m_Vertices[i3].normal += triangle_normal;
    i1 = i2;
    i2 = i3;
    order = !order;
  }
  for(unsigned int i = 0; i<m_Vertices.size();i++){
    m_Vertices[i].normal = normalize(m_Vertices[i].normal);
  } 

}


template <class T>
void Model<T>::Init(unsigned int renderType, std::string shaderPath)
{
  m_RendererType = renderType;
  Push();

  m_Va = new VertexArray();
  m_Vb = new VertexBuffer((const void *) &m_Vertices[0], m_Vertices.size() *  sizeof(T));
  m_Shader = new Shader(shaderPath);

  m_Va->AddBuffer(*m_Vb, m_Layout);

  m_Ib = new IndexBuffer((const unsigned int *) &m_Indices[0], m_Indices.size());


  m_Va->Unbind();
  m_Vb->Unbind();
  m_Ib->Unbind();
  m_Shader->Unbind();
}


template <class T>
Model<T>::Model()
{
}

template <class T>
Model<T>::Model(unsigned int renderType, std::vector<T>& positions, std::vector<unsigned int>& indices, std::string shaderPath)
	: m_RendererType(renderType),  m_Vertices(positions),
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
	m_Ib->Bind();
	m_Shader->Bind();
}

template <class T>
void Model<T>::Unbind()
{
	m_Va->Unbind();
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
void Model<T>::SetTexture(const std::string path, const std::string name, unsigned int slot)
{
  switch(Texture::ParseFormat(path)){
    case TEX_DDS:
      m_Texture = new DDSTexture(path);
      break;
    case TEX_OTHER:
      m_Texture = new OtherTexture(path);
      break;
  }
  InitTexture(name, slot);
}

template <class T>
void Model<T>::SetTexture(const std::string path)
{
  // degueu
  switch(Texture::ParseFormat(path)){
    case TEX_DDS:
      m_Texture = new DDSTexture(path);
      break;
    case TEX_OTHER:
      m_Texture = new OtherTexture(path);
      break;
  }
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

template <class T>
void Model<T>::InitTexture(const std::string name, unsigned int id)
{
	m_Shader->Bind();
	m_Texture->Bind(id);
	m_Shader->SetUniform1i(name, id);
//	m_Texture->Unbind();
	m_Shader->Unbind();
}

#endif
