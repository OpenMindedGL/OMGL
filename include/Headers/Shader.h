#ifndef Shader_H
#define Shader_H
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

#include <glm/glm.hpp>

#include "GL/glew.h"


struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};


class Shader 
{

private : 
	std::string m_FilePath;
	unsigned int m_RendererId;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public : 
	Shader();
	Shader(const std::string& filepath);
	//Shader(const char* filepath);
	~Shader();

	void Bind() const; 
	void Unbind() const; 

	//Set uniforms
	void SetUniform1i(const std::string& name, int value);
        void SetUniform2i(const std::string & name, glm::i32vec2 value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
        void SetUniform3f(const std::string& name, glm::vec3 v);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, glm::mat4 & matrix);

private : 
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
};
#endif
