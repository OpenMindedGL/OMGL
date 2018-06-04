#ifndef Renderer_H
#define Renderer_H
#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Camera.h"
#include "Debug.h"
#include "Terrain.h"

class Skybox;
class Object;
class Renderer
{
private:

	Camera * m_Camera;

public:
	Renderer(GLFWwindow & window, int w, int h);

	void Clear() const;

	void Draw(Object object);

	void Draw(std::vector<Object> objects);

	void Draw(Skybox s);
	void Draw(Terrain& t);

	inline glm::vec3 getCameraPosition() const { return m_Camera->getPosition(); };
};
#endif
