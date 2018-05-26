#ifndef Camera_H
#define Camera_H
#pragma once
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class GravityObject;

class Camera 
{
private: 
	GLFWwindow & m_Window;
	int m_Width; 
	int m_Height;

	float m_HorizontalAngle; 
	float m_VerticalAngle;
	float m_InitialFoV;
	float m_Speed;
	float m_MouseSpeed;

	glm::vec3 m_Position;

	glm::mat4 m_Model;
	glm::mat4 m_View;
	glm::mat4 m_Proj; 
	glm::mat4 m_MVP; 

        float m_DistFromChar;
        float m_AngleAroundChar;
        GravityObject * m_Character;
        bool m_CursorRelease;

public:
	Camera(GLFWwindow & window, int w, int h);

	void ComputeMatricesFromInputs();

	inline void ComputeMVP() { m_MVP = m_Proj * m_View * m_Model; }

	inline void SetModel(glm::mat4 model) { m_Model = model; }

	inline glm::mat4 GetModel() const { return m_Model; }
	inline glm::mat4 GetView() const { return m_View; }
	inline glm::mat4 GetProj() const { return m_Proj; }
	inline glm::mat4 GetMVP() const { return m_MVP; }
	inline glm::vec3 getPosition() const { return m_Position; }

	inline void setPosition(glm::vec3 new_pos) { m_Position = new_pos; }
	inline void setHorizontalAngle(float new_angle) { m_HorizontalAngle = new_angle; }
	inline void setVerticalAngle(float new_angle) { m_VerticalAngle = new_angle; }
	inline void setCharacter(GravityObject * new_char) { m_Character = new_char; }
	
	void printCoord();
        void Move(int xpos, int ypos);
};
#endif
