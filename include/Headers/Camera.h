#pragma once
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class Camera 
{
private: 
	GLFWwindow & m_Window;
	int m_Width; 
	int m_Height;

	float m_HorizontalAngle; 
	float m_VertivaleAngle;
	float m_InitialFoV;
	float m_Speed;
	float m_MouseSpeed;

	glm::vec3 m_Position;

	glm::mat4 m_Model;
	glm::mat4 m_View;
	glm::mat4 m_Proj; 
	glm::mat4 m_MVP; 

public:
	Camera(GLFWwindow & window, int w, int h);

	void ComputeMatricesFromInputs();

	inline void ComputeMVP() { m_MVP = m_Proj * m_View * m_Model; }

	inline void SetModel(glm::mat4 model) { m_Model = model; }

	inline glm::mat4 GetModel() const { return m_Model; }
	inline glm::mat4 GetView() const { return m_View; }
	inline glm::mat4 GetProj() const { return m_Proj; }
	inline glm::mat4 GetMVP() const { return m_MVP; }
	
	void printCoord();
};