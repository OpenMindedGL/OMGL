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

class LODLevel { 

  private:

    static Mesh<Vertexun> m_Tile;
    static Mesh<Vertexun> m_Fill;
    static Mesh<Vertexun> m_Trim;
    static Mesh<Vertexun> m_Seam;

    static void GenMeshes(unsigned int size);
    static void GenTile();
    static void GenFill();
    static void GenTrim();
    static void GenSeam();


    unsigned int m_Level;

    /* pre-computed */
    static int m_Size;
    static int m_TileSize;
    static int m_HalfSize;
    static int m_QuarterSize;
    static int m_DoubleSize;
    glm::i32vec2 m_TorEnd;    
    /*              */

    std::vector<Object&> m_Objs;
    std::vector<Object> m_TileObjs;
    Object m_FillObjs[4];
    Object m_TrimObj;
    Object m_SeamObj;

    glm::i32vec2 m_TorBegin;    // origin in the toroid array
    Terrain * m_Terrain;

    // 
    int m_UnitSize;
    glm::i32vec2 m_ActiveR;
    glm::i32vec2 m_NewActiveR;
    
    void SetOffset( glm::i32vec2& o );
    void SetScale(int s);
    int Load();

  public:

    void GridIndices(glm::i32vec2& e, Mesh<Vertexun>& m);
    void ComputeIndices();
    void Update(glm::i32vec2 center);
    void PutVertex(glm::i32vec2& pos);
    LODLevel(unsigned int l, glm::vec2& center, Terrain* t);
    int GetIndex(glm::i32vec2& p);


    // getter
    inline Object&
};

#endif
