#ifndef LODLEVEL_H
#define LODLEVEL_H

class LODLevel;
class Terrain;

#include <string>
#include <vector>
#include <stack>
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Vertex.h"
#include "NoiseGen.h"
#include "Terrain.h"

class LODLevel : public Mesh<Vertexun> { 

  private:

    unsigned int m_Level;

    /* pre-computed */
    static unsigned int** pre2D1D;
    int m_Size;
    int m_HalfSize;
    int m_QuarterSize;
    int m_DoubleSize;
    int m_UnitSize;
    glm::i32vec2 m_TorEnd;    
    /*              */

    unsigned int m_UploadStart;
    unsigned int m_UploadCount;
    Vertexun* m_MappedBuffer;
    glm::i32vec2 m_TorBegin;    // origin in the toroid array
    Terrain * m_Terrain;
    //glm::vec2 m_ClipR;
    glm::i32vec2 m_ActiveR;
    glm::i32vec2 m_NewActiveR;
    void MapBuffer();
    void UnmapBuffer();
    int Load();
    void IndicesArea(glm::i32vec2& s, glm::i32vec2& e);

  public:

    static void Make2D1D(unsigned int s);
    void ComputeIndices();
    void Update(glm::i32vec2 center);
    void PutVertex(glm::i32vec2& pos);
    LODLevel(unsigned int l, glm::vec2& center, Terrain* t);
    int GetIndex(glm::i32vec2& p);
};

#endif
