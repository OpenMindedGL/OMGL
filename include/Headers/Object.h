#pragma once

#include "Model.h"
#include "Vertex.h"
#include "Material.h"
#include <vector>
#include <string>
#include <map>

class Object : public Model<Vertexun>
{
private : 
	std::vector<Material> m_Materials;
	std::map<unsigned int, std::string> m_ListMat;

public : 
	Object();
	Object(std::string pathObj, std::string pathMtl, bool reverse);
	
	bool LoadObject(std::string pathObj, std::string pathMtl, bool reverse);
	int contains(glm::vec3 v, glm::vec2 vt, glm::vec3 vn);
	void InitMaterials(std::string path, std::vector<Material> & listMat);
};