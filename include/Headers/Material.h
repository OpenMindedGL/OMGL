#pragma once
#include <string>
#include "Texture.h"

class Material {

private : 
	
	std::string m_Name;
	float m_Ns;
	float m_Ni;
	float m_D;
	glm::vec3 m_Ka;
	glm::vec3 m_Kd;
	glm::vec3 m_Ks; 
	glm::vec3 m_Ke; 
	unsigned int m_illum;
	Texture m_Texture;

public :
	Material();
	Material(std::string name);

	//Setters
	inline void setName(std::string name) { m_Name = name; }
	inline void setNs(float ns) { m_Ns = ns; }
	inline void setKa(glm::vec3 ka) { m_Ka = ka; }
	inline void setKd(glm::vec3 kd) { m_Kd = kd; }
	inline void setKs(glm::vec3 ks) { m_Ks = ks; }
	inline void setKe(glm::vec3 ke) { m_Ke = ke; }
	inline void setNi(float ni) { m_Ni = ni; }
	inline void setD(float d) { m_D = d; }
	inline void setIllum(unsigned int illum) { m_illum = illum; }

	//Getters
	inline std::string getName() { return m_Name; }
	inline float getNs() { return m_Ns; }
	inline glm::vec3 getKa() { return m_Ka; }
	inline glm::vec3 getKd() { return m_Kd; }
	inline glm::vec3 getKs() { return m_Ks; }
	inline glm::vec3 getKe() { return m_Ke; }
	inline float getNi() { return m_Ni; }
	inline float getD() { return m_D; }
	inline Texture getTexture() { return m_Texture; }

};