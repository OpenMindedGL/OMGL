#ifndef Tree_H
#define Tree_H
#pragma once

#include "Model.h"
#include "NoiseGen.h"


class Tree : public Model<Vertexun> {
private : 
  FastNoise noise;
  void generateBranch(float length, float radius, glm::vec3 origin, glm::vec3 direction);
  void compute_indices();
public : 
	Tree();
        void generate(long int seed);

};
#endif

