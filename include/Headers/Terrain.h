#ifndef Terrain_H
#define Terrain_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Model.h"
#include "Vertex.h"
#include "NoiseGen.h"

#define NB_LEVELS 32
#define CHUNK_PER_SIDE 5   // KEEP ODD !!!!
#define RENDER_DISTANCE (CHUNK_PER_SIDE/2)
#define TERRAIN_SZ (CHUNK_PER_SIDE*CHUNK_SIZE)

class Terrain {

  private :
    float m_Precision;
    unsigned int m_InitSize;
    LODLevel lods[NB_LEVELS];   // Future Objects


  public :
    Terrain();


}

class LODLevel : public Model<Vertexun>{ // Future Mesh

  private:

    unsigned int m_Level;
    unsigned int m_Size;

    // Pre-computed
    unsigned int m_Halfsize;
    float m_Precision;
    

  public:

    Update(glm::vec2 center);
    LODLevel(unsigned int l,);
    PutVertex(glm::i32vec2 pos, Vertexun v);
}


#endif
