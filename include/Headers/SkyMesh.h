#ifndef SkyMesh_H
#define SkyMesh_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Vertex.h"

#include "Mesh.h"
#include "YGen.h"
#include "OMGLNoiseGen.h"

#define CHUNK_SIZE 32
#define CHUNK_PER_SIDE 5
#define RENDER_DISTANCE (CHUNK_PER_SIDE/2)
#define TERRAIN_SZ (CHUNK_PER_SIDE*CHUNK_SIZE)

class SkyMesh : public Mesh<Vertexun>
{
  private: 
    glm::i32vec2 buffer_map[CHUNK_PER_SIDE*CHUNK_PER_SIDE];

    glm::i32vec2 last_chunk; 
    
    float precision;
    YGen* m_VNoise;
    YGen* m_RNoise;

    // returns chunk coords for point
    glm::i32vec2 GetChunk( glm::vec2 pointCoords );

  public: 

    SkyMesh(glm::vec2 center, YGen* rectoNoise, YGen* versoNoise);
    
    void initload_sky(glm::vec2 center);
    void load(glm::vec2 coords);
    void compute_indices_sky();
    void compute_normals();

};

#endif
