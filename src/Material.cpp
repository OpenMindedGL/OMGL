#include "Material.h"
unsigned int Material::NBINSTANCES  = 0;

Material::Material(std::string name) : m_Name(name), m_Id(++NBINSTANCES)
{
	m_Shader = new Shader(BASICSHADER);
	m_Shader->Unbind();
}

Material::Material(Texture * texture, Shader * shader) :
	m_Texture(texture), m_Shader(shader), m_Id(++NBINSTANCES)
{
  if(m_Texture && m_Shader)
    m_Texture->LinkToShader(m_Shader);
}

Material::Material(float * color) 
{
	m_Ka = glm::vec3(0.15f, 0.15f, 0.15f);
	m_Kd = glm::vec3(color[0], color[1], color[2]);
	m_Ks = glm::vec3(0.0f);
	m_Ke = glm::vec3(0.0f);
	m_Ni = 0;
	m_D = 1;
	m_Ns = 1;
}

void Material::GenerateShader(std::string shadersPath, std::string genShaderPath)
{
	ShaderGenerator gen(m_DynamicUniforms, shadersPath, genShaderPath);
	m_Shader = new Shader(shadersPath+"/"+gen.getName());
	m_Shader->Unbind();
}

void Material::SetShader(Shader* shader)
{
  m_Shader = shader;
  if(m_Texture)
    m_Texture->LinkToShader(m_Shader);
}

void Material::SetTexture(Texture * texture)
{
  m_Texture  = texture;
  if(m_Shader)
    m_Texture->LinkToShader(m_Shader);
}

void Material::LinkTexture(const std::string& name, unsigned int slot){
    m_Texture->LinkToShader(m_Shader, name, slot);
}

void Material::SetShaderUniformMat4f(const std::string name, glm::mat4 mvp)
{
  m_Shader->SetUniformMat4f(name, mvp);
}

void Material::BindTextures()
{
	m_Shader->Bind();
	int it = -1;
	if (GetMapKd() != NULL) {
		GetMapKd()->Bind(++it);
		m_Shader->SetUniform1i("u_KdMap", it);
	}
	else m_Shader->SetUniform3f("u_Kd", m_Kd.x, m_Kd.y, m_Kd.z);

	if (GetMapKs() != NULL) {
		GetMapKs()->Bind(++it);
		m_Shader->SetUniform1i("u_KsMap", it);
	}
	else m_Shader->SetUniform3f("u_Ks", m_Ks.x, m_Ks.y, m_Ks.z);

	if (GetMapKa() != NULL) {
		GetMapKa()->Bind(++it);
		m_Shader->SetUniform1i("u_KaMap", it);
	}
	else m_Shader->SetUniform3f("u_Ka", m_Ka.x, m_Ka.y, m_Ka.z);

	if (GetMapNs() != NULL) {
		GetMapNs()->Bind(++it);
		m_Shader->SetUniform1i("u_Ns", it);
	}
	else m_Shader->SetUniform1f("u_Ns", m_Ns);

	if (GetMapD() != NULL) {
		GetMapD()->Bind(++it);
		m_Shader->SetUniform1i("u_DMap", it);
	}
	else m_Shader->SetUniform1f("u_D", m_D);

	m_Shader->SetUniform1f("u_Ni", m_Ni);
	m_Shader->SetUniform3f("u_Ke", m_Ke.x, m_Ke.y, m_Ke.z);
	m_Shader->SetUniform1i("u_Illum", m_illum);
}

void Material::Init(std::string shaderpath)
{
  m_Shader = new Shader(shaderpath);
  m_Texture = NULL;
}

void Material::Init()
{
	m_Shader = new Shader();
	m_Texture = NULL;
}

void Material::Bind()
{
  if (m_Texture != NULL)
    m_Texture->Bind();
  m_Shader->Bind();
}
void Material::Unbind()
{
  if (m_Texture != NULL)
    m_Texture->Unbind();
  m_Shader->Unbind();
}
