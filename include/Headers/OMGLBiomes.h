#ifndef OMGLBiomes_H
#define OMGLBiomes_H

#include "Biome.h"
#include "Material.h"
#include "OMGLNoiseGen.h"

class OMGLMountains : public Biome{

  public:
    OMGLMountains();
};

class OMGLDesert : public Biome{

  public:
    OMGLDesert() : Biome(new OMGLNoiseBiome3(), 500, -500) {
    
      BiomeMaterial* sand = new BiomeMaterial(
          0.0001f,
          0.3f,
          glm::vec3(0.5f),
          glm::vec3(0.3f,0.3f,0.1f),
          glm::vec3(0.5f,0.5f,0.1f)
          );
      std::vector<BiomeMaterial>* a = new std::vector<BiomeMaterial>();
      a->push_back(*sand);
      AltitudeBiome* def = new AltitudeBiome(
            0.0, 
            a
            );
      m_AltBiomes = new std::vector<AltitudeBiome>(); 
      m_AltBiomes->push_back(*def);
    
    }

};
class OMGLBlocky : public Biome{

  public:
    OMGLBlocky() : Biome(new OMGLNoiseBiome2(), 200, 0) {}

};
class OMGLPeanuts : public Biome{

  public:
    OMGLPeanuts() : Biome(new OMGLNoiseBiome4(), 200, 0) {}

};
#endif

