#pragma once
#include <stdio.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <vector>

#include "Camera.h"

class VertexArray;
class IndexBuffer;
class Shader;
class Cube;


#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError(); 

bool GLLogCall(const char* function, const char* file, int line);


class Renderer
{
private : 

	Camera* m_Camera;

public : 
	Renderer(GLFWwindow & window, int w, int h);

	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader);
	void Draw(Cube cube);
	void Draw(std::vector<Cube> cubes);
};