#ifndef Tree_H
#define Tree_H
#pragma once

#include "Model.h"


class Tree : public Model<Vertexun> {
private : 

public : 
	Tree();
        void generate(long int seed);
        void generateBranch(float length, float radius, glm::vec3 origin, glm::vec3 direction, const unsigned int& indoffset);
};
#endif

