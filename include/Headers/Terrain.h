#pragma once

#include <string>
#include <vector>
#include "Vertex.h"
#include "Model.h"

#define CHUNK_SIZE 32
#define LENGTH 3


typedef struct Chunk { Vertexun v[CHUNK_SIZE][CHUNK_SIZE]; } Chunk;

class Terrain : public Model<Vertexun>
{

  private : 
    std::vector<unsigned int> indices;
    std::vector<Chunk> chunks; 
    int buffer_map[LENGTH][2];
    std::string shaderpath;


  public: 

    Terrain();
    ~Terrain();
    
    void initload(glm::vec2 center);
    void load(glm::vec2 chunkcoords);
    void compute_indices();

};

