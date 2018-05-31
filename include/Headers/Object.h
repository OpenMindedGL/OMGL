#pragma once

#include <vector>
#include <map>
#include <string>
#include "Material.h"
#include "Mesh.h"

class Object {


private : 

	glm::mat4 m_ModelMatrix;
	glm::mat4 m_Position;
	glm::mat4 m_Rotation;

protected : 

	Mesh<Vertexun> * m_Mesh;
	std::vector<Material*> m_Materials;
	std::map<unsigned int, std::string> m_ListMat;

public :
	Object();
	Object(Mesh<Vertexun> * m);
	Object(Mesh<Vertexun> * m, Material * mat);
	Object(Mesh<Vertexun> * m, std::vector<Material*> mat);
	Object(Mesh<Vertexun> * m, std::string shaderPath);
	Object(std::string pathObj, std::string pathMtl, bool reverse);

	void LoadObject(std::string pathObj, std::string pathMtl, bool reverse);
	void InitMaterials(std::string path, std::vector<Material*> & materials);
	int contains(glm::vec3 v, glm::vec2 vt, glm::vec3 vn);
	bool hasMaterial(Material* mat);

	void Init(unsigned int renderType, std::string shaderPath);
	void Bind();
	void Unbind();
	
	void Translate(glm::vec3 position);
	void Rotation(float angle, glm::vec3 axis);
	void Scale(glm::vec3 scale);

	//getters
	inline std::vector<Material*> GetMaterials() { return m_Materials; }
	inline Mesh<Vertexun> * GetMesh() { return m_Mesh; }
	inline glm::mat4 GetModelMatrix() const { return m_ModelMatrix; }

};