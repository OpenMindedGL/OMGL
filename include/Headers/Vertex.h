#ifndef Vertex_H
#define Vertex_H
#ifndef CLASS_VERTEX
#define CLASS_VERTEX

#include <glm/glm.hpp>

class Vertex {
public:
	static unsigned int nbattrib;
	glm::vec3 pos;
	Vertex(glm::vec3 p);
	Vertex(float x, float y, float z);
	~Vertex();
};


class Vertexu : public Vertex {
public:
	static unsigned int nbattrib;
	glm::vec2 uv;

	Vertexu(glm::vec3 p, glm::vec2 u);
	~Vertexu();
};

class Vertexun : public Vertex {
public:
	static unsigned int nbattrib;
	glm::vec2 uv;
	glm::vec3 normal;

	Vertexun(glm::vec3 p, glm::vec2 u, glm::vec3 n);
	~Vertexun();
};
/*
class Vertexf {
public:
	static unsigned int nbattrib;
	glm::vec2 uv;
	glm::vec3 normal;

	Vertexun(glm::vec3 p, glm::vec2 u, glm::vec3 n);
	~Vertexun();
};*/
#endif
#endif
