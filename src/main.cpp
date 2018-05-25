// Allow vec3(1,2,3).xz
//#define GLM_SWIZZLE


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
#include <glm/gtc/matrix_transform.hpp>

//.h
#include "Cube.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Terrain.h"
#include "Skybox.h"


using namespace glm;


#define WIDTH 1920 
#define HEIGHT 1080

int init();
GLFWwindow* window;

int init(){

  srand((unsigned int)time(0));

  if(!glfwInit())
    return -1;

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

  // Needed by some drivers/hardware
  glewExperimental = GL_TRUE; 

  if (glewInit() != GLEW_OK)
    return -1;
  //std::cout << "Error!" << std::endl;

  std::cout << glGetString(GL_VERSION) << std::endl;

  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  // Enable depth test
  GLCall(glEnable(GL_DEPTH_TEST));
  // Accept fragment if it closer to the camera than the former one
  GLCall(glDepthFunc(GL_LESS));
}


int main(void){

  init();
  Renderer renderer(*window, WIDTH, HEIGHT);

  glm::vec3 pos = renderer.getCameraPosition();
  Terrain t(glm::vec2(pos.x,pos.z));
  Cube c("shaders/Cube.shader");
  c.SetTexture("textures/grass.jpg", "u_TextureSampler");
  Skybox s("shaders/Skybox.shader");
  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
  {

    renderer.Clear();

    pos = renderer.getCameraPosition();
    //printf("x:%f y:%f \n",pos.x,pos.z);
    t.load(glm::vec2(pos.x,pos.z));


    renderer.Draw<Vertexun>(c);
    renderer.Draw<Vertexun>(t);
    renderer.Draw(s);


    /* Swap frint and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process envents */
    glfwPollEvents();
  }
  //glDeleteTextures(1,c.GetTexture()->m_RendererID);
  glfwTerminate();
  return 0;
}

