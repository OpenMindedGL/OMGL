#include "Renderer.h"
#include "Skybox.h"

Renderer::Renderer(GLFWwindow & window, int w, int h)
	: m_Camera(new Camera(window, w, h))
{}

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(Object object)
{
	m_Camera->ComputeMatricesFromInputs();
	m_Camera->SetModel(object.GetModelMatrix());
	m_Camera->ComputeMVP();
	glm::mat4 mvp = m_Camera->GetMVP();

	object.Bind();

	if (object.GetMesh()->GetRendererType() == GL_TRIANGLE_STRIP) {
		GLCall(glEnable(GL_PRIMITIVE_RESTART));
		GLCall(glPrimitiveRestartIndex(object.GetMesh()->GetVertices()->size()));
	}
	else {
		GLCall(glDisable(GL_PRIMITIVE_RESTART));
	}

	for (int i = 0; i < object.GetMaterials().size(); i++) {
		object.GetMaterials().at(i)->SetShaderUniformMat4f("u_MVP", mvp);
		object.GetMaterials().at(i)->SetShaderUniformMat4f("u_M", object.GetModelMatrix());
		object.GetMaterials().at(i)->SetShaderUniformMat4f("u_V", m_Camera->GetView());
	}

	GLCall(glDrawElements(object.GetMesh()->GetRendererType(), object.GetMesh()->GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr));

	object.Unbind();
}

void Renderer::Draw(std::vector<Object> objects) {

	m_Camera->ComputeMatricesFromInputs();
	m_Camera->printCoord();

	for (int i = 0; i < objects.size(); i++) {
		m_Camera->SetModel(objects[i].GetModelMatrix());
		m_Camera->ComputeMVP();
		glm::mat4 mvp = m_Camera->GetMVP();
		objects[i].Bind();
		for (int j = 0; j < objects[i].GetMaterials().size(); j++) {
			objects[i].GetMaterials().at(j)->SetShaderUniformMat4f("u_MVP", mvp);
		}
		GLCall(glDrawElements(objects[i].GetMesh()->GetRendererType(), objects[i].GetMesh()->GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr));
		objects[i].Unbind();
	}
}

void Renderer::Draw(Skybox s)
{
	m_Camera->ComputeMatricesFromInputs();
	m_Camera->SetModel(s.GetModelMatrix());
	m_Camera->SetView(glm::mat4(glm::mat3(m_Camera->GetView())));
	m_Camera->ComputeMVP();
	glm::mat4 mvp = m_Camera->GetMVP();

	s.Bind();

	if (s.GetMesh()->GetRendererType() == GL_TRIANGLE_STRIP) {
		GLCall(glEnable(GL_PRIMITIVE_RESTART));
		GLCall(glPrimitiveRestartIndex(s.GetMesh()->GetVertices()->size()));
	}
	else {
		GLCall(glDisable(GL_PRIMITIVE_RESTART));
	}

	s.GetMaterials().at(0)->SetShaderUniformMat4f("u_MVP", mvp);
	s.GetMaterials().at(0)->SetShaderUniformMat4f("u_M", s.GetModelMatrix());
	s.GetMaterials().at(0)->SetShaderUniformMat4f("u_V", m_Camera->GetView());

	// draw skybox behind everything
	glDepthFunc(GL_LEQUAL);
	GLCall(glDrawElements(s.GetMesh()->GetRendererType(), s.GetMesh()->GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr));

	glDepthFunc(GL_LESS);
	s.Unbind();
}
