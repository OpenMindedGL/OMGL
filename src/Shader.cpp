#include "Shader.h"
#include "Debug.h"


Shader::Shader()
{
}

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererId(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	m_RendererId = CreateShader(source.VertexSource, source.FragmentSource);
	printf("render id shader : %d\n", m_RendererId);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererId));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererId));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string & name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform2i(const std::string & name, glm::i32vec2 value)
{
	GLCall(glUniform2i(GetUniformLocation(name), value.x, value.y));
}

void Shader::SetUniform1f(const std::string & name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform3f(const std::string & name, glm::vec3 v)
{
  SetUniform3f(name, v.x,v.y,v.z);
}

void Shader::SetUniform3f(const std::string & name, float v0, float v1, float v2)
{
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string & name, glm::mat4 & matrix)
{
	glm::mat4 mvp = matrix;
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mvp[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
  //  printf("getting uniform location for %s ! \n", name.c_str());
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(m_RendererId, name.c_str()));
	if (location == -1)
		printf("Warning uniform %s doesn't exist ! \n", name.c_str());
	
	m_UniformLocationCache[name] = location;

	return location;
}


ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE; 
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result; 
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length; 
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		printf("Failed to compile %s \n", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"));
		printf("%s\n", message);
		GLCall(glDeleteShader(id));

		return 0;
	}
	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}
