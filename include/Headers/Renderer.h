#ifndef Renderer_H
#define Renderer_H
#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Camera.h"
#include "Debug.h"


class VertexArray;
class IndexBuffer;
class Shader;
class Cube;

template <class T>
class Model;

class Renderer
{
private : 

	Camera* m_Camera;

public : 
	Renderer(GLFWwindow & window, int w, int h);

	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader);
	//void Draw(Cube cube);

        template <class T>
	void Draw(Model<T> model);
	
        void Draw(std::vector<Cube> cubes);
   
        template <class T>
        void Draw(std::vector<Model<T>> models);

        inline glm::vec3 getCameraPosition() const{ return m_Camera->getPosition(); };
};

template <class T>
void Renderer::Draw(Model<T> model)
{
	m_Camera->ComputeMatricesFromInputs();
	m_Camera->SetModel(model.GetModelMatrix());
	m_Camera->ComputeMVP();
	glm::mat4 mvp = m_Camera->GetMVP();

	model.Bind();

    if(model.GetRendererType() == GL_TRIANGLE_STRIP){
      GLCall(glEnable(GL_PRIMITIVE_RESTART));
      GLCall(glPrimitiveRestartIndex(model.GetVertices().size()));
    }
    else{
      GLCall(glDisable(GL_PRIMITIVE_RESTART));
    }

	model.SetShaderUniformMat4f("u_MVP", mvp);
	model.SetShaderUniformMat4f("u_M", model.GetModelMatrix());
	model.SetShaderUniformMat4f("u_V", m_Camera->GetView());

	GLCall(glDrawElements(model.GetRendererType(), model.GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr));

	model.Unbind();
}


template <class T>
void Renderer::Draw(std::vector<Model<T>> models) {
	m_Camera->ComputeMatricesFromInputs();
	m_Camera->printCoord();

	for (int i = 0; i < models.size(); i++) {
		m_Camera->SetModel(models[i].GetModel());
		m_Camera->ComputeMVP();
		glm::mat4 mvp = m_Camera->GetMVP();
		models[i].Bind();
		models[i].SetShaderUniformMat4f("u_MVP", mvp);
		GLCall(glDrawElements(models[i].GetRendererType(), models[i].GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr));
		models[i].Unbind();

	}
}

#endif
