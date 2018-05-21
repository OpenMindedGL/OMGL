#include "Renderer.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Cube.h"
#include "Model.h"



Renderer::Renderer(GLFWwindow & window, int w, int h)
	: m_Camera(new Camera(window, w, h))
{
}

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader)
{
	m_Camera->ComputeMatricesFromInputs();
	m_Camera->ComputeMVP();
	
	glm::mat4 mvp = m_Camera->GetMVP();

	shader.Bind();
	shader.SetUniformMat4f("u_MVP", mvp);

	va.Bind();
	ib.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

}

