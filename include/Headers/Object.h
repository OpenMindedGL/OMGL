#pragma once

#include <vector>
#include <map>
#include <string>
#include "Material.h"
#include "Mesh.h"

class Object {

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

	void Init(unsigned int renderType, std::string shaderPath);
	void Bind();
	void Unbind();
	
	int contains(glm::vec3 v, glm::vec2 vt, glm::vec3 vn);
	bool hasMaterial(Material* mat);

	//getters
	inline std::vector<Material*> GetMaterials() { return m_Materials; }
	inline Mesh<Vertexun> * GetMesh() { return m_Mesh; }

};