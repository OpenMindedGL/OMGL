#ifndef Terrain_H
#define Terrain_H
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Vertex.h"
#include "Model.h"
#include "NoiseGen.h"

#define CHUNK_SIZE 32
#define CHUNK_PER_SIDE 3
#define TERRAIN_SZ (CHUNK_PER_SIDE*CHUNK_SIZE)
#define RENDER_DISTANCE (TERRAIN_SZ/2)



class Terrain : public Model<Vertex>
{

  private : 

    glm::i32vec2 buffer_map[CHUNK_PER_SIDE*CHUNK_PER_SIDE];

    glm::i32vec2 last_chunk; 
    
    float precision;
    NoiseGen noise;

    void loadchunk(glm::i32vec2 coords, glm::i32vec2 replace);
    int getOffset(glm::i32vec2 coords);


  public: 

    Terrain();
    
    void initload(glm::vec2 center);
    void load(glm::vec2 coords);
    void compute_indices();

};

#endif
