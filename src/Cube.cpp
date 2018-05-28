#include "Cube.h"
#include <string>

Cube::Cube(std::string shaderPat){
      
	m_Vertices = {
		Vertexun(glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),     // 0
		Vertexun(glm::vec3(-1.0f,1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)),     // 1
		Vertexun(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),    // 2
		Vertexun(glm::vec3(1.0f, 1.0f,-1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)),      // 3
		Vertexun(glm::vec3(-1.0f,-1.0f,1.0f), glm::vec2(0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f)),      // 4
		Vertexun(glm::vec3(-1.0f,1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),      // 5
		Vertexun(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f)),     // 6
		Vertexun(glm::vec3(1.0f, 1.0f,1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),       // 7
	};



	m_Indices = {
		0, 2,
		1, 3,
		5, 7,
		4, 6, 8,
		1, 5,
		0, 4,
		2, 6,
		3, 7,
	};
        Init(GL_TRIANGLE_STRIP,shaderPat);

}
