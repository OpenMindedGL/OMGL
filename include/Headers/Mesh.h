#ifndef Mesh_H
#define Mesh_H


#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Vertex.h"
#include "VertexBufferLayout.h"

template <class T>
class Mesh
{

protected:

	std::vector<T> * m_Vertices;
	std::vector<unsigned int> * m_Indices;

private:

	unsigned int m_RendererType;

	VertexBuffer * m_Vb;
	VertexArray * m_Va;
	VertexBufferLayout m_Layout;
	IndexBuffer * m_Ib;

	void Push();

public:

	Mesh();
	Mesh(unsigned int renderType, std::vector<T>& positions, std::vector<unsigned int>& indices, std::string shaderPath);

	void Bind();
	void Unbind();

	void Upload();
	void UploadIndexBuffer();
        void Upload(unsigned int start, unsigned int count);

	void Init(unsigned int renderType);

	void ComputeNormals();
	inline void BindVertexBuffer() { m_Vb->Bind(); };
        void GridIndices(glm::i32vec2& e, glm::i32vec2 s=glm::i32vec2(0), unsigned int yoffset = 0);


	// GETTERS
	inline VertexArray & GetVertexArray() const { return *m_Va; }
	inline IndexBuffer & GetIndexBuffer() const { return *m_Ib; }
	inline VertexBuffer & GetVertexBuffer() const { return *m_Vb; }
	inline unsigned int GetRendererType() const { return m_RendererType; }

	inline std::vector<T> * GetVertices() const { return m_Vertices; }
	inline std::vector<unsigned int> * GetIndices() const { return m_Indices; }

        // SETTERS

        inline void PushVertex(T v){m_Vertices->push_back(v); }
        inline void PushIndex(unsigned int i){m_Indices->push_back(i); }

};

template <class T>
void Mesh<T>::GridIndices(glm::i32vec2& e, glm::i32vec2 s, unsigned int yoff){
  glm::i32vec2 ind = s;
  int yoffset = yoff;      // offset of the line
  int yoffsetn = yoffset;     // of the next line
  while( ind.y < e.y-1 ){
    yoffsetn+=e.x;
    while( ind.x < e.x ){
      m_Indices->push_back(yoffset+ind.x);
      m_Indices->push_back(yoffsetn+ind.x);
      ind.x++;
    }
    m_Indices->push_back(m_Vertices->size());
    ind.y++;
    ind.x = 0;
    yoffset+=e.x;
  }
}

template <class T>
void Mesh<T>::ComputeNormals() {

	unsigned int i1 = m_Indices->at(0);
	unsigned int i2 = m_Indices->at(1);
	unsigned int i3;
	glm::vec3 triangle_normal;
	bool order = true;
	for (unsigned int i = 2; i < m_Indices->size() - 1; i++) {
		if (m_Indices->at(i) == m_Vertices->size()) {
			// reached end of line (index == restart index)
			i1 = m_Indices->at(++i);
			i2 = m_Indices->at(++i);
			i++;
			order = true;
		}
		i3 = m_Indices->at(i);
		// compute cross product in right order
		if (order)
			triangle_normal = normalize(cross(m_Vertices->at(i2).pos - m_Vertices->at(i1).pos, m_Vertices->at(i3).pos - m_Vertices->at(i1).pos));
		else
			triangle_normal = normalize(cross(m_Vertices->at(i2).pos - m_Vertices->at(i3).pos, m_Vertices->at(i1).pos - m_Vertices->at(i3).pos));
		m_Vertices->at(i1).normal += triangle_normal;
		m_Vertices->at(i2).normal += triangle_normal;
		m_Vertices->at(i3).normal += triangle_normal;
		i1 = i2;
		i2 = i3;
		order = !order;
	}
	for (unsigned int i = 0; i < m_Vertices->size(); i++) {
		m_Vertices->at(i).normal = normalize(m_Vertices->at(i).normal);
	}
}

template <class T>
void Mesh<T>::Init(unsigned int renderType)
{
	m_RendererType = renderType;
	Push();
	m_Va = new VertexArray();
	m_Vb = new VertexBuffer((const void *)&m_Vertices->at(0), m_Vertices->size() * sizeof(T));

	m_Va->AddBuffer(*m_Vb, m_Layout);

	m_Ib = new IndexBuffer((const unsigned int *)&m_Indices->at(0), m_Indices->size());

	m_Va->Unbind();
	m_Vb->Unbind();
	m_Ib->Unbind();
}

template <class T>
Mesh<T>::Mesh()
{
	m_Vertices = new std::vector<T>;
	m_Indices = new std::vector<unsigned int>;
}

template <class T>
Mesh<T>::Mesh(unsigned int renderType, std::vector<T>& positions, std::vector<unsigned int>& indices, std::string shaderPath)
	: m_RendererType(renderType), m_Vertices(positions),
	m_Indices(indices)//, m_ModelMatrix(glm::mat4(1.0f))
{
	Push();

	m_Va = new VertexArray();
	m_Vb = new VertexBuffer(&positions, positions.size() * sizeof(T));
	m_Va->AddBuffer(*m_Vb, m_Layout);
	m_Ib = new IndexBuffer((const unsigned int *)&indices[0], indices.size());

	m_Va->Unbind();
	m_Vb->Unbind();
	m_Ib->Unbind();
}

template <class T>
void Mesh<T>::Bind()
{
	m_Va->Bind();
	m_Ib->Bind();
}

template <class T>
void Mesh<T>::Unbind()
{
	m_Va->Unbind();
	m_Ib->Unbind();
}

template <class T>
void Mesh<T>::Upload() {
	(*m_Vb).Upload();
}

template <class T>
void Mesh<T>::Upload(unsigned int start, unsigned int count) {
	(*m_Vb).Upload(start*sizeof(T),count*sizeof(T));
}

template <class T>
void Mesh<T>::UploadIndexBuffer() {
	(*m_Ib).Upload();
}



#endif
