#pragma once

#include "Model.h"
#include "Vertex.h"
#include <string>

class Object : public Model<Vertexun>
{
private : 

public : 
	Object();
	Object(std::string path);

	bool LoadObject(std::string path);
	int contains(glm::vec3 v, glm::vec2 vt, glm::vec3 vn);
};