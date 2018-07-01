#pragma once
#include <string>
#include "Texture.h"
#include "Shader.h"
#include "Cubemap.h"
#include "ShaderGen.h"

#define BASICSHADER "shaders/Basic.shader"

class Material {

private : 
	
	static unsigned int NBINSTANCES;

	std::string m_Name;

	unsigned int m_Id;
	unsigned int m_illum;

	float m_Ns;
	float m_Ni;
	float m_D;

	glm::vec3 m_Ka;
	glm::vec3 m_Kd;
	glm::vec3 m_Ks;
	glm::vec3 m_Ke;

	Shader * m_Shader = NULL;
	Texture * m_Texture = NULL,  * m_MapKd = NULL,  *m_MapKs = NULL,  *m_MapKa = NULL,  *m_MapNs = NULL,  *m_MapD = NULL;

	std::vector<bool> m_DynamicUniforms = { false, false, false, false, false };
	
public :

	//Constructeurs
	Material(std::string name);
	Material(Texture * texture = NULL, Shader * shader = new Shader(BASICSHADER));
	Material(float * color);
    inline Material(std::string texture, std::string shader) : Material(new Texture(texture), new Shader(shader)) {}
    inline Material(Shader * shader) : Material(NULL, shader) {}

	//Genere les shaders automatiquement
	void GenerateShader(std::string shadersPath, std::string genShaderPath);

	//Permet de lier une texture au shader
	void LinkTexture(const std::string& name, unsigned int slot);

	//Permet de d�finir un uniform au format mat4f (matrice 4x4 de float)
	void SetShaderUniformMat4f(const std::string name, glm::mat4 mvp);

	//Pour lier les textures au shader, et cr�er les uniforms correspondant (pour la g�n�ration de shader automatique)
	void BindTextures();

	void Init(std::string shaderpath);
	void Init();
	void Bind();
	void Unbind();

	//Setters
	void SetShader(Shader * shader);
	void SetTexture(Texture * texture);
	inline void SetName(std::string name) { m_Name = name; }
	inline void SetNs(float ns) { m_Ns = ns; }
	inline void SetKa(glm::vec3 ka) { m_Ka = ka; }
	inline void SetKd(glm::vec3 kd) { m_Kd = kd; }
	inline void SetKs(glm::vec3 ks) { m_Ks = ks; }
	inline void SetKe(glm::vec3 ke) { m_Ke = ke; }
	inline void SetNi(float ni) { m_Ni = ni; }
	inline void SetD(float d) { m_D = d; }
	inline void SetIllum(unsigned int illum) { m_illum = illum; }
	inline void SetMapKd(std::string path) { m_MapKd = new Texture(path); }
	inline void SetMapKs(std::string path) { m_MapKs = new Texture(path); }
	inline void SetMapKa(std::string path) { m_MapKa = new Texture(path); }
	inline void SetMapNs(std::string path) { m_MapNs = new Texture(path); }
	inline void SetMapD(std::string path) { m_MapD = new Texture(path); }
	inline void SetShader(const std::string path) { SetShader(new Shader(path)); }

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
	inline Shader* GetShader() { return m_Shader; }
	inline unsigned int GetId() { return m_Id; }
	inline Texture* GetMapKd() { return m_MapKd; }
	inline Texture* GetMapKs() { return m_MapKs; }
	inline Texture* GetMapKa() { return m_MapKa; }
	inline Texture* GetMapNs() { return m_MapNs; }
	inline Texture* GetMapD() { return m_MapD; }
	inline std::vector<bool> & GetDynamicsUniforms() { return m_DynamicUniforms; }

};
