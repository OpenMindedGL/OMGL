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
#include "DynamicHeightMap.h"

/* for now 10 kinda stutters when it needs to update them all in a frame
 * 8 is alright
 * Multithreading would solve the problem
 */
#define NB_LEVELS 10


#define PRECISION 1.0f
#define SIZE    131   // -3 should be power of two, dividable by 8
#define HALFSIZE    SIZE/2

class Terrain {

  private :
    unsigned int m_NbLevels;
    float m_Precision;
    int m_Size;
    float m_Scale;
    glm::i32vec2 m_Center;
    
    DynamicHeightMap* m_HeightMap;
    LODLevel * m_Lods[NB_LEVELS];
    Material * m_Material;
    Shader * m_Shader;
    Texture * m_NormalMap;
    NoiseGen m_Noise;


  public :
    /* should really be private */ 
    /*                  */

    Terrain(glm::vec2 spawn, float p = PRECISION, unsigned int s = SIZE, unsigned int n = NB_LEVELS);
    inline int GetSize(){ return m_Size; }
    inline float GetPrecision(){ return m_Precision; }
    inline unsigned int GetNbLevel(){ return m_NbLevels; }
    inline LODLevel& GetLevel(unsigned int i){ return *(m_Lods[i]); }
    void Update(glm::i32vec2& center);

    // getters
    inline unsigned int GetNbLevels(){ return m_NbLevels; }
    inline Material* GetMaterial(){ return m_Material; }
    inline DynamicHeightMap* GetHeightMap(){ return m_HeightMap;}
    inline LODLevel ** GetLods(){ return m_Lods;}
    inline Shader * GetShader(){ return m_Shader;}
    inline Texture * GetNormalMap(){ return m_NormalMap;}
    inline NoiseGen * GetNoise(){ return &m_Noise;}


};



#endif
