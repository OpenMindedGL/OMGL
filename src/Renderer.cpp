#include "Renderer.h"
#include "Skybox.h"
#include <map>


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

	if(m_Camera->getTypeDisplay()) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

	//If severals mtl
	if (object.GetListMat()->size() > 1) {
		std::map<unsigned int, unsigned int>::iterator it = object.GetListMat()->begin();
		unsigned int tmp = 0;
		while (true) {
			//OBEJCT.GETMATERIALS.SETUNIFORM(it->second...)
			object.GetMaterials().at(it->second)->SetUniforms();
			++it;
			GLCall(glDrawElements(
				object.GetMesh()->GetRendererType(), //type
				(it == object.GetListMat()->end() ? object.GetMesh()->GetIndexBuffer().GetCount() - tmp: it->first - tmp + 1),//count
				GL_UNSIGNED_INT,
				(const void *)(tmp * sizeof(unsigned int)) //offset indice 
			));
			if (it == object.GetListMat()->end()) break;
			tmp = it->first + 1;
		}
	}
	else {
		GLCall(glDrawElements(object.GetMesh()->GetRendererType(), object.GetMesh()->GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr));
	}
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



void Renderer::Draw(Terrain& t)
{
  Object& l = t.GetLevel(0);
  for (int i = 0; i < t.GetNbLevel(); i++) {
    l = t.GetLevel(i);
    Draw(l);
  }
}
