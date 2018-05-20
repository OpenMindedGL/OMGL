#ifndef Terrain_H
#define Terrain_H
#include <string>
#include <vector>
#include "Vertex.h"
#include "Model.h"
#include "NoiseGen.h"

#define CHUNK_SIZE 32
#define NB_CHUNK_PER_SIDE 3
#define TERRAIN_SZ (NB_CHUNK_PER_SIDE*CHUNK_SIZE)
#define RENDER_DISTANCE (TERRAIN_SZ/2)



class Terrain : public Model<Vertex>
{

  private : 
    int buffer_map[NB_CHUNK_PER_SIDE][2];
    NoiseGen noise;


  public: 

    Terrain();
    
    void initload(glm::vec2 center);
    void load(glm::vec2 chunkcoords);
    void compute_indices();

};

#endif
