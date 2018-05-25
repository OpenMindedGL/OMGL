#include "Renderer.h"

#include "Skybox.h"
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
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
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

void Renderer::Draw(Skybox s)
{
	m_Camera->ComputeMatricesFromInputs();
	m_Camera->SetModel(s.GetModelMatrix());
	m_Camera->SetView(glm::mat4(glm::mat3(m_Camera->GetView())));
	m_Camera->ComputeMVP();
	glm::mat4 mvp = m_Camera->GetMVP();

	s.Bind();

        if(s.GetRendererType() == GL_TRIANGLE_STRIP){
          GLCall(glEnable(GL_PRIMITIVE_RESTART));
          GLCall(glPrimitiveRestartIndex(s.GetVertices().size()));
        }
        else{
          GLCall(glDisable(GL_PRIMITIVE_RESTART));
        }

	s.SetShaderUniformMat4f("u_MVP", mvp);
	s.SetShaderUniformMat4f("u_M", s.GetModelMatrix());
	s.SetShaderUniformMat4f("u_V", m_Camera->GetView());


	GLCall(glDrawElements(s.GetRendererType(), s.GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr));

	s.Unbind();
}
