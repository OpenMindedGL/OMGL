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

#define NB_LEVELS 1
#define PRECISION 1.0f
#define SIZE    4
#define HALFSIZE    SIZE/2
#define CHUNK_PER_SIDE 5   // KEEP ODD !!!!
#define RENDER_DISTANCE (CHUNK_PER_SIDE/2)
#define TERRAIN_SZ (CHUNK_PER_SIDE*CHUNK_SIZE)

class Terrain {

  private :
    unsigned int m_NbLevels;
    float m_Precision;
    int m_Size;
    int m_HalfSize;
    Object lods_obj[NB_LEVELS];   // Future Objects
    LODLevel * lods[NB_LEVELS];
    Material * m_Material;


  public :
    Terrain(glm::vec2 spawn, float p = PRECISION, unsigned int s = SIZE, unsigned int n = NB_LEVELS);
    inline int GetSize(){ return m_Size; }
    inline float GetPrecision(){ return m_Precision; }
    inline unsigned int GetNbLevel(){ return m_NbLevels; }
    inline Object& GetLevel(unsigned int i){ return lods_obj[i]; }
    void Update(glm::i32vec2& center);


};

class LODLevel : public Mesh<Vertexun> { 

  private:

    /* pre-computed */
    static unsigned int** pre2D1D;
    unsigned int m_UnitSize;
    int m_Size;
    int m_HalfSize;
    int m_DoubleSize;
    glm::i32vec2 m_TorEnd;    
    /*              */

    unsigned int m_UploadStart;
    unsigned int m_UploadCount;
    unsigned int m_Level;
    glm::i32vec2 m_TorBegin;    // origin in the toroid array
    Terrain * m_Terrain;
    //glm::vec2 m_ClipR;
    glm::i32vec2 m_ActiveR;
    glm::i32vec2 m_NewActiveR;

  public:

    static void Make2D1D(unsigned int s);
    void ComputeIndices();
    int Update(glm::i32vec2 center);
    void PutVertex(glm::i32vec2& pos);
    LODLevel(unsigned int l, glm::vec2& center, Terrain* t);
    int GetIndex(glm::i32vec2& p);
};


#endif
