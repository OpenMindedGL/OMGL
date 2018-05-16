//Includes necessaires
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <signal.h>
#include <time.h>

//Glew
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

//.h
#include "Cube.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"


using namespace glm;

#define WIDTH 960 
#define HEIGHT 540

int main(void){

	srand((unsigned int)time(0));

	if(!glfwInit())
		return -1;
	
	GLFWwindow* window;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a wubdower mode window and its OpenGL context 
	window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
	if(!window){
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		return -1;
		//std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	{

		float positions[] = {
			-1.0f,-1.0f,-1.0f, //0.0f, 0.0f,  // 0
			-1.0f,-1.0f, 1.0f, //1.0f, 0.0f,  // 1 
			-1.0f, 1.0f, 1.0f, //1.0f, 1.0f,  // 2
			 1.0f, 1.0f,-1.0f, //0.0f, 1.0f,  // 3
			-1.0f, 1.0f,-1.0f, //0.0f, 0.0f,  // 4
			 1.0f,-1.0f, 1.0f, //1.0f, 0.0f,  // 5
			 1.0f,-1.0f,-1.0f, //1.0f, 1.0f,  // 6
			 1.0f, 1.0f, 1.0f, //0.0f, 1.0f   // 7
		};		

		unsigned int indices[] = {
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

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		Cube cube(GL_TRIANGLES, positions, indices, "res/shaders/Basic.shader");

		Cube cube2(GL_TRIANGLES, positions, indices, "res/shaders/Basic.shader");
		
		vec3 translation(10.0f, 1.0f, 0.0f);

		cube2.Translate(translation);
		cube2.Rotation(15.0f, vec3(0.0f, 0.0f, 1.0f));
		cube2.Rotation(-15.0f, vec3(1.0f, 0.0f, 0.0f));
		cube2.Scale(vec3(5.0f, 5.0f, 5.0f));


		std::vector<Cube> cubes(500);
		
		for (int i = 0; i < cubes.size(); i++) {
			cubes[i] = Cube(GL_TRIANGLE_STRIP, positions, indices, "res/shaders/Basic.shader");
			cubes[i].Translate( vec3(
				(rand() / (float) RAND_MAX) * 200 - 100,
				(rand() / (float)RAND_MAX) * 200 - 100,
				(rand() / (float)RAND_MAX) * 200 - 100
			));
		}
	
		Renderer renderer(*window, WIDTH, HEIGHT);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
		{
			
			/* Render here */
			renderer.Clear();
			
			renderer.Draw(cubes);

			/*for (int i = 0; i < cubes.size(); i++) {
				cubes[i].Rotation(0.1f, vec3(1.0f, 1.0f, 1.0f));
			}*/

			//renderer.Draw(cube.GetVertexArray(), cube.GetIndexBuffer(), cube.GetShader());
			//renderer.Draw(cube);
			//renderer.Draw(cube2);

			//cube2.Rotation(0.05f, vec3(1.0f, 0.0f, 1.0f));

			/* Swap frint and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process envents */
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}

