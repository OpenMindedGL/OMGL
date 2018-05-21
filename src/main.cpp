//Includes necessaires
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <signal.h>
#include <time.h>
#include <math.h>
#include <vector>

//Glew
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

//.h
#include "Vertex.h"
#include "Renderer.h"
#include "Cube.h"

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

	Vertex va(1.0, 1.0, 1.0);

	std::cout << glGetString(GL_VERSION) << std::endl;
	{

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		Cube cube(GL_TRIANGLES, "res/shaders/Basic.shader");
		cube.Init();

		Cube cube2(GL_TRIANGLES, "res/shaders/Basic.shader");
		
		vec3 translation(5.0f, 0.0f, 0.0f);

		cube2.Translate(translation);

		//cube2.Rotation(15.0f, vec3(0.0f, 0.0f, 1.0f));
		//cube2.Rotation(-15.0f, vec3(1.0f, 0.0f, 0.0f));
		/*cube2.Scale(vec3(5.0f, 5.0f, 5.0f));
		cube.Scale(vec3(2.5f, 2.5f, 2.5f));
		cube.Scale(vec3(2.0f, 2.0f, 2.0f));
		cube.Scale(vec3(-5.0f, -5.0f, -5.0f));
		*/

		//Model<unsigned int> m(indices);
		//Model<Vertex> m(positions);

		/*std::vector<Cube> cubes(500);
		
		for (int i = 0; i < cubes.size(); i++) {
			cubes[i] = Cube(GL_TRIANGLE_STRIP, "res/shaders/Basic.shader");
			cubes[i].Translate( vec3(
				(rand() / (float) RAND_MAX) * 200 - 100,
				(rand() / (float)RAND_MAX) * 200 - 100,
				(rand() / (float)RAND_MAX) * 200 - 100
			));
		}*/
		
		Renderer renderer(*window, WIDTH, HEIGHT);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
		{
			
			/* Render here */
			renderer.Clear();
			
			//renderer.Draw(cubes);

			//renderer.Draw(cube.GetVertexArray(), cube.GetIndexBuffer(), cube.GetShader());
			
			renderer.Draw(cube);
			//renderer.Draw(cube2);
			
			//cube2.Rotation(0.05f, vec3(1.0f, 0.0f, 1.0f));

			//renderer.Draw(cube2);


			/* Swap frint and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process envents */
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}

