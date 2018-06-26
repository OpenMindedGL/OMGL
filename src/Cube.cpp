#include "Cube.h"
#include <string>

Cube::Cube(){
      
	m_Vertices = new std::vector<Vertexun> {
		Vertexun(glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),     // 0
		Vertexun(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),    // 2
		Vertexun(glm::vec3(-1.0f,1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)),     // 1
		Vertexun(glm::vec3(1.0f, 1.0f,-1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)),      // 3

		Vertexun(glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),     // 0
		Vertexun(glm::vec3(-1.0f,1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)),     // 1
		Vertexun(glm::vec3(-1.0f,-1.0f,1.0f), glm::vec2(0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f)),      // 4
		Vertexun(glm::vec3(-1.0f,1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),      // 5

		Vertexun(glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),     // 0
		Vertexun(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),    // 2
		Vertexun(glm::vec3(-1.0f,-1.0f,1.0f), glm::vec2(0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f)),      // 4
		Vertexun(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f)),     // 6

		Vertexun(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f)),     // 6
		Vertexun(glm::vec3(1.0f, 1.0f,1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),       // 7
		Vertexun(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),    // 2
		Vertexun(glm::vec3(1.0f, 1.0f,-1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)),      // 3


		Vertexun(glm::vec3(1.0f, 1.0f,-1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)),      // 3
		Vertexun(glm::vec3(1.0f, 1.0f,1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),       // 7
		Vertexun(glm::vec3(-1.0f,1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)),     // 1
		Vertexun(glm::vec3(-1.0f,1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),      // 5

		Vertexun(glm::vec3(-1.0f,1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),      // 5
		Vertexun(glm::vec3(-1.0f,-1.0f,1.0f), glm::vec2(0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f)),      // 4
		Vertexun(glm::vec3(1.0f, 1.0f,1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),       // 7
		Vertexun(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f))     // 6
	};

	unsigned int restartIndex = m_Vertices->size();
	m_Indices = new std::vector<unsigned int>{
		0, 1, 2, 3, restartIndex,
		4, 5, 6, 7, restartIndex,
		8, 9, 10, 11, restartIndex,
		12, 13, 14, 15, restartIndex,
		16, 17, 18, 19, restartIndex,
		20, 21, 22, 23, restartIndex
	};

	ComputeNormals();
    Init(GL_TRIANGLE_STRIP);
}
