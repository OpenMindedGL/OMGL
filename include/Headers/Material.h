#pragma once
#include <string>
#include "Texture.h"
#include "Shader.h"
#include "Cubemap.h"

class Material {

private : 
	
	static unsigned int NBINSTANCES;

	unsigned int m_Id;
	std::string m_Name;
	float m_Ns;
	float m_Ni;
	float m_D;
	glm::vec3 m_Ka;
	glm::vec3 m_Kd;
	glm::vec3 m_Ks; 
	glm::vec3 m_Ke; 
	unsigned int m_illum;
	Texture * m_Texture;
	Shader * m_Shader;

public :
	Material(std::string name);
	Material(Texture * texture = NULL, Shader * shader = NULL);
        inline Material(std::string texture, std::string shader) : Material(new Texture(texture), new Shader(shader)) {}
        inline Material(Shader * shader) : Material(NULL, shader) {}

	inline void SetShader(const std::string path) { SetShader(new Shader(path)); }
	void SetShader(Shader * shader);
	void SetTexture(Cubemap * cubemap);
	void SetTexture(Texture * texture);

	void LoadTexture(const std::string path, const std::string name, unsigned int slot = 0);
	void LoadTexture(const std::string path);
	void LinkTexture(const std::string& name, unsigned int slot);
	void SetShaderUniformMat4f(const std::string name, glm::mat4 mvp);
	void SetUniforms();
	void Init(std::string shaderpath);
	void Bind();
	void Unbind();

	//Setters
	inline void SetName(std::string name) { m_Name = name; }
	inline void SetNs(float ns) { m_Ns = ns; }
	inline void SetKa(glm::vec3 ka) { m_Ka = ka; }
	inline void SetKd(glm::vec3 kd) { m_Kd = kd; }
	inline void SetKs(glm::vec3 ks) { m_Ks = ks; }
	inline void SetKe(glm::vec3 ke) { m_Ke = ke; }
	inline void SetNi(float ni) { m_Ni = ni; }
	inline void SetD(float d) { m_D = d; }
	inline void SetIllum(unsigned int illum) { m_illum = illum; }

	//Getters
	inline std::string GetName() { return m_Name; }
	inline float GetNs() { return m_Ns; }
	inline glm::vec3 GetKa() { return m_Ka; }
	inline glm::vec3 GetKd() { return m_Kd; }
	inline glm::vec3 GetKs() { return m_Ks; }
	inline glm::vec3 GetKe() { return m_Ke; }
	inline float GetNi() { return m_Ni; }
	inline float GetD() { return m_D; }
	inline Texture* GetTexture() { return m_Texture; }
	inline unsigned int GetId() { return m_Id; }

};
