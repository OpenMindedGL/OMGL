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
#include "DynamicBiomeHeightMap.h"
#include "Sampler.h"
#include <math.h>

/* TODO :
 * precomputed 1D2D
 * Precision support
 * Double (Triple ?) texturing
 * Multithreaded loading
 */

class LODLevel { 

  private:

    static Mesh<Vertexun> m_Tile;
    static Mesh<Vertexun> m_Fill[4];
    static Mesh<Vertexun> m_Trim;
    static Mesh<Vertexun> m_Seam;
    static Mesh<Vertexun> m_Cross;

    static void GenTile();
    static void GenFill();
    static void GenCross();
    static void GenTrim();
    static void GenSeam();



    /* pre-computed */
    static int m_Size;
    static int m_TileSize;
    static int m_HalfSize;
    static int m_QuarterSize;
    static int m_DoubleSize;
    glm::i32vec2 m_TorEnd;    
    static unsigned int** pre2D1D;
    /*              */

    std::vector<Object*> m_Objs;
    std::vector<Object*> m_TileObjs;
    std::vector<Object*> m_FillObjs;
    Object* m_TrimObj;
    Object* m_SeamObj;
    Object* m_CrossObj;

    Terrain * m_Terrain;
    Material * m_Material;
    DynamicBiomeHeightMap* m_HeightMap;
    Sampler* m_HeightMapLinear;
    LODLevel* m_Previous;

    // 
    unsigned int m_Level;
    int m_UnitSize;
    glm::i32vec2 m_ActiveR;
    glm::i32vec2 m_NewActiveR;
    
    // Should really put it in Mesh, or make a Grid class
    static void Grid(glm::i32vec2 s, glm::i32vec2 e, Mesh<Vertexun>* m);

    void MakeObjs();
    void MakeTileObjs();
    void MakeTileObj(glm::i32vec2& pos);
    void MakeFillObjs();
    void MakeFillObj(glm::i32vec2& pos, Mesh<Vertexun>* m);
    void MakeCrossObj();
    void MakeTrimObj(glm::i32vec2& pos, glm::vec3& rot);
    void MakeSeamObj();
    void PlaceTrim();
    //int Load();

    void ColorDebug();

  public:


    void SetUniforms();
    //void GridIndices(glm::i32vec2& e, Mesh<Vertexun>& m);
    static void GenMeshes(unsigned int size);
    void Update(glm::i32vec2 center);
    LODLevel(unsigned int l, glm::vec2& center, Terrain* t, LODLevel* prev);
    //int GetIndex(glm::i32vec2& p);


    // getter
    inline std::vector<Object*>& GetTiles() { return m_TileObjs; }
    inline std::vector<Object*>& GetFills() { return m_FillObjs; }
    inline Object* GetTrim() { return m_TrimObj; }
    inline Object* GetSeam() { return m_SeamObj; }
    inline std::vector<Object*>& GetObjs() { return m_Objs; }
    inline int GetUnitSize() { return m_UnitSize; }
    inline DynamicBiomeHeightMap* GetHeightMap() { return m_HeightMap; }
    inline Sampler* GetHeightMapLinear() { return m_HeightMapLinear; }
};

#endif
