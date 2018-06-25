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
#include "ProcMixer.h"
#include "Material.h"
#include "LODLevel.h"
#include "Biome.h"
#include "DynamicHeightMap.h"

#define NB_LEVELS 10
#define DEFAULT_SCALE   64
#define DEFAULT_SPAWN   glm::i32vec2(0,0)


#define PRECISION 1.0f
#define SIZE    131   // -3 should be power of two, dividable by 8
#define HALFSIZE    SIZE/2

class Terrain {

  private :
    unsigned int m_NbLevels;
    int m_Size;
    float m_Scale;
    glm::i32vec2 m_Center;
    
//    DynamicHeightMap* m_HeightMap;
    LODLevel * m_Lods[NB_LEVELS];
    Material * m_Material;
    Shader * m_Shader;
  //  Texture * m_NormalMap;
    ProcMixer * m_Mixer;
    std::vector<Biome*>* m_Biomes;

    static std::vector<YGen*>* GetNoises(std::vector<Biome*>* biomes);

  public :

      Terrain(
          std::vector<Biome*>* biomes,
          glm::vec2 spawn = DEFAULT_SPAWN,
          unsigned int s = SIZE,
          unsigned int n = NB_LEVELS,
          float scale = DEFAULT_SCALE
          ) : 
        Terrain(
            biomes,
            new ProcMixer(Terrain::GetNoises(biomes)),
            s,
            n,
            scale
            ) {}

    Terrain(
        std::vector<Biome*>* biomes,
        ProcMixer* mixer,
        glm::vec2 spawn = DEFAULT_SPAWN,
        unsigned int s = SIZE,
        unsigned int n = NB_LEVELS,
        float scale = DEFAULT_SCALE
        );


    void Update(glm::i32vec2& center);

    // getters
    inline int GetSize(){ return m_Size; }
    inline unsigned int GetNbLevel(){ return m_NbLevels; }
    inline LODLevel& GetLevel(unsigned int i){ return *(m_Lods[i]); }
    inline unsigned int GetNbLevels(){ return m_NbLevels; }
    inline Material* GetMaterial(){ return m_Material; }
//    inline DynamicHeightMap* GetHeightMap(){ return m_HeightMap;}
    inline LODLevel ** GetLods(){ return m_Lods;}
    inline Shader * GetShader(){ return m_Shader;}
    inline YGen* GetNoise(){ return m_Mixer; }
//    inline Texture * GetNormalMap(){ return m_NormalMap;}


};



#endif
