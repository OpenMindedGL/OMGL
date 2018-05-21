#pragma once

#include "Model.h"
#include "Vertex.h"

class Cube : public Model<Vertexun>
{

private : 

	std::vector<Vertexun> m_InitVertexUN = {
		Vertexun(glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
		Vertexun(glm::vec3(-1.0f,-1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
		Vertexun(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
		Vertexun(glm::vec3( 1.0f, 1.0f,-1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
		Vertexun(glm::vec3(-1.0f, 1.0f,-1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
		Vertexun(glm::vec3( 1.0f,-1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
		Vertexun(glm::vec3( 1.0f,-1.0f,-1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
		Vertexun(glm::vec3( 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f))
	};


	std::vector<unsigned int> m_Indices = {
		0, 1, 2,
		3, 0, 4,
		5, 0, 6,
		3, 6, 0,
		0, 2, 4,
		5, 1, 0,
		2, 1, 5,
		7, 6, 3,
		6, 7, 5,
		7, 3, 4,
		7, 4, 2,
		7, 2, 5
	};

public: 
	Cube();
	Cube(unsigned int m_RendererType, std::string shaderPath);
	//vector<T> & getTypePosition();
};


/*
template<typename T>
inline T Cube<T>::getTypePosition()
{
	assert(false);
};

template<>
inline vector<Vertex> & Cube<Vertex>::getTypePosition()
{
	return m_InitVertex;
};

template<>
inline vector<Vertexu> & Cube<Vertexu>::getTypePosition()
{
	return m_InitVertexU;
};

template<>
inline vector<Vertexun> Cube<Vertexun>::getTypePosition()
{
	return m_InitVertexUN;
};*/

/*
static vector<Vertex> m_InitVertex[] = {
Vertex(-1.0f,-1.0f,-1.0f),
Vertex(-1.0f,-1.0f, 1.0f),
Vertex(-1.0f, 1.0f, 1.0f),
Vertex(1.0f, 1.0f,-1.0f),
Vertex(-1.0f, 1.0f,-1.0f),
Vertex(1.0f,-1.0f, 1.0f),
Vertex(1.0f,-1.0f,-1.0f),
Vertex(1.0f, 1.0f, 1.0f)
};

/*
static vector<Vertexu> m_InitVertexU[] = {
Vertexu(glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec2(0.0f, 0.0f)),
Vertexu(glm::vec3(-1.0f,-1.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
Vertexu(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
Vertexu(glm::vec3(1.0f, 1.0f,-1.0f),  glm::vec2(0.0f, 0.0f)),
Vertexu(glm::vec3(-1.0f, 1.0f,-1.0f), glm::vec2(0.0f, 0.0f)),
Vertexu(glm::vec3(1.0f,-1.0f, 1.0f),  glm::vec2(0.0f, 0.0f)),
Vertexu(glm::vec3(1.0f,-1.0f,-1.0f),  glm::vec2(0.0f, 0.0f)),
Vertexu(glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec2(0.0f, 0.0f))
};*/