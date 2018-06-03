#include "Material.h"
unsigned int Material::NBINSTANCES  = 0;

Material::Material() : m_Id(++NBINSTANCES)
{}

Material::Material(std::string name) : m_Name(name), m_Id(++NBINSTANCES)
{}

Material::Material(Texture * texture, Shader * shader) : m_Texture(texture), m_Shader(shader), m_Id(++NBINSTANCES)
{}

Material::Material(Texture * texture) : m_Texture(texture), m_Id(++NBINSTANCES)
{}

Material::Material(Cubemap * cubemap) : m_Texture(cubemap), m_Id(++NBINSTANCES)
{}

Material::Material(Shader * shader) : m_Shader(shader), m_Id(++NBINSTANCES)
{
	m_Texture = NULL;
}

void Material::SetShader(const std::string path)
{
	m_Shader = new Shader(path);
	m_Shader->Unbind();
}

void Material::SetTexture(Cubemap * cubemap)
{
	m_Texture = cubemap;
	m_Texture->Unbind();
}

void Material::SetTexture(Texture * texture)
{
	m_Texture  = texture;
	m_Texture->Unbind();
}

void Material::LoadTexture(const std::string path, const std::string name, unsigned int slot)
{
	switch (Texture::ParseFormat(path)) {
	case TEX_DDS:
		m_Texture = new DDSTexture(path);
		break;
	case TEX_OTHER:
		m_Texture = new OtherTexture(path);
		break;
	}
	InitTexture(name, slot);
}

void Material::LoadTexture(const std::string path)
{
	// degueu
	switch (Texture::ParseFormat(path)) {
	case TEX_DDS:
		m_Texture = new DDSTexture(path);
		break;
	case TEX_OTHER:
		m_Texture = new OtherTexture(path);
		break;
	}
}

void Material::InitTexture(const std::string name, unsigned int id)
{
	m_Shader->Bind();
	m_Texture->Bind(id);
	m_Shader->SetUniform1i(name, id);
	m_Texture->Unbind();
	m_Shader->Unbind();
}

void Material::SetShaderUniformMat4f(const std::string name, glm::mat4 mvp)
{
	m_Shader->SetUniformMat4f(name, mvp);
}

void Material::Init(std::string shaderpath)
{
	m_Shader = new Shader(shaderpath);
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
