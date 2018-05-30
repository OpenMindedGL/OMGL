#ifndef Terrain_H
#define Terrain_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Vertex.h"
#include "Model.h"
#include "NoiseGen.h"

#define CHUNK_SIZE 32
#define CHUNK_PER_SIDE 5   // KEEP ODD !!!!
#define RENDER_DISTANCE (CHUNK_PER_SIDE/2)
#define TERRAIN_SZ (CHUNK_PER_SIDE*CHUNK_SIZE)



class Terrain : public Model<Vertexun>
{

  private : 

    glm::i32vec2 buffer_map[CHUNK_PER_SIDE*CHUNK_PER_SIDE];

    glm::i32vec2 last_chunk; 
    
    float precision;
    NoiseGen noise;

    void loadchunk(glm::i32vec2 coords, glm::i32vec2 replace, std::vector<glm::i32vec2> sides);
    
    // returns offset of chunk "coords" in gpu vertex buffer
    int getOffset(glm::i32vec2 coords);
    
    void ComputeNormals(glm::i32vec2 chunk_coords, Vertexun * vertices, unsigned int * indices, unsigned int offset);
    
    glm::i32vec2 Surround(glm::i32vec2 chunk, unsigned int count);
    
    // returns chunk coords for point
    glm::i32vec2 GetChunk( glm::vec2 pointCoords );

    // returns bottom left point
    glm::vec2 GetPoint( glm::i32vec2 chunkCoords );
    
    bool IsLoaded(glm::i32vec2 chunk);

    glm::i32vec2 surroundings[4];
/*      glm::i32vec2(0,-1),
      glm::i32vec2(0,1),
      glm::i32vec2(1,0),
      glm::i32vec2(-1,0)
    };
*/

  public: 

    Terrain(glm::vec2 center);
    
    void initload(glm::vec2 center);
    void load(glm::vec2 coords);
    void compute_indices();

};



#endif
