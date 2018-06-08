#ifndef Terrain_H
#define Terrain_H

class LODLevel;

#include <string>
#include <vector>
#include <stack>
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Object.h"
#include "Vertex.h"
#include "NoiseGen.h"
#include "Material.h"
#include "LODLevel.h"

#define NB_LEVELS 7
#define PRECISION 1.0f
#define SIZE    129   // -1 should be power of two, dividable by 8
#define HALFSIZE    SIZE/2
#define CHUNK_PER_SIDE 5   // KEEP ODD !!!!
#define RENDER_DISTANCE (CHUNK_PER_SIDE/2)
#define TERRAIN_SZ (CHUNK_PER_SIDE*CHUNK_SIZE)

class Terrain {

  private :
    unsigned int m_NbLevels;
    float m_Precision;
    int m_Size;
    LODLevel * lods[NB_LEVELS];
    Material * m_Material;


  public :
    
    NoiseGen m_Noise;
    Terrain(glm::vec2 spawn, float p = PRECISION, unsigned int s = SIZE, unsigned int n = NB_LEVELS);
    inline int GetSize(){ return m_Size; }
    inline float GetPrecision(){ return m_Precision; }
    inline unsigned int GetNbLevel(){ return m_NbLevels; }
    inline Object& GetLevel(unsigned int i){ return lods_obj[i]; }
    void Update(glm::i32vec2& center);


};



#endif
