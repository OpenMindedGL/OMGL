#include "Renderer.h"
#include "Camera.h"

#include <iostream>


Camera::Camera(GLFWwindow & window, int w, int h)
	: m_Window(window), m_HorizontalAngle(3.14f), m_VertivaleAngle(0.0f), m_InitialFoV(45.0f),
	  m_Speed(3.0f), m_MouseSpeed(0.002f), m_Position(glm::vec3(1, 30, 1)), m_Model(glm::mat4(1.0)), 
	  m_View(glm::mat4(0.0)), m_MVP(glm::mat4(1.0)), m_Width(w), m_Height(h)
{
}

void Camera::ComputeMatricesFromInputs()
{
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();

	float deltaTime = float(currentTime - lastTime) * 5;

	// Get mouse position
	double xpos, ypos;
	GLCall(glfwGetCursorPos(&m_Window, &xpos, &ypos));

	// Reset mouse position for next frame
	GLCall(glfwSetCursorPos(&m_Window, m_Width / 2, m_Height/ 2));

	// Compute new orientation
	m_HorizontalAngle += m_MouseSpeed* float(m_Width/ 2 - xpos);
	m_VertivaleAngle += m_MouseSpeed* float(m_Height / 2 - ypos);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(m_VertivaleAngle) * sin(m_HorizontalAngle),
		sin(m_VertivaleAngle),
		cos(m_VertivaleAngle) * cos(m_HorizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(m_HorizontalAngle - 3.14f / 2.0f),
		0,
		cos(m_HorizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
        glm::vec3 up = glm::cross(right, direction);


        if (glfwGetKey(&m_Window, GLFW_KEY_W) == GLFW_PRESS) {
          if (glfwGetKey(&m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            m_Position.y += deltaTime * m_Speed;
          else 
            m_Position += direction * deltaTime * m_Speed ;
        }

        if (glfwGetKey(&m_Window, GLFW_KEY_S) == GLFW_PRESS) {
          if (glfwGetKey(&m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            m_Position.y -= deltaTime * m_Speed;
          else
            m_Position -= direction * deltaTime * m_Speed;
        }

        if (glfwGetKey(&m_Window, GLFW_KEY_D) == GLFW_PRESS) {
          m_Position += right * deltaTime * m_Speed;
        }

        if (glfwGetKey(&m_Window, GLFW_KEY_A) == GLFW_PRESS) {
          m_Position -= right * deltaTime * m_Speed;
        }

	float FoV = m_InitialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	m_Proj = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 1000.0f);
	// Camera matrix
	m_View = glm::lookAt(
		m_Position,             // Camera is here
		m_Position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// For the next frame, the "last time" will be "now"

	lastTime = currentTime;
}

void Camera::printCoord()
{
	printf("pos : %f x, %f y, %f z\n", m_Position.x, m_Position.y, m_Position.z);
}
