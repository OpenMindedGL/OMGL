#include "OMGLBiomes.h"
OMGLMountains::OMGLMountains()  : 
  Biome(new OMGLNoiseBiome1(), 1000,-1000)
{
  BiomeMaterial* rock = new BiomeMaterial(
      0.6f,
      1.0f,
      glm::vec3(0.1f),
      glm::vec3(0.3f),
      glm::vec3(0.4f)
      );
  BiomeMaterial* snow = new BiomeMaterial(
      0.0f,
      3.5f,
      glm::vec3(0.1f),
      glm::vec3(0.7f),
      glm::vec3(0.9f)
      );
  std::vector<BiomeMaterial>* b = new std::vector<BiomeMaterial>();
  b->push_back(*snow);
  b->push_back(*rock);
  AltitudeBiome* tops = new AltitudeBiome(
      0.53, 
      b
      );


  BiomeMaterial* dirt = new BiomeMaterial(
      0.3f,
      1.0f,
      glm::vec3(0.1f),
      glm::vec3(0.3f,0.22f,0),
      glm::vec3(0.2f,0.1f,0)
      );
  BiomeMaterial* grass = new BiomeMaterial(
      0.0f,
      1.0f,
      glm::vec3(0.1f),
      glm::vec3(0.1f,0.31f,0.1f)*0.8f,
      glm::vec3(0.0)
      );
  std::vector<BiomeMaterial>* a = new std::vector<BiomeMaterial>();
  a->push_back(*grass);
  a->push_back(*dirt);
  AltitudeBiome* valleys = new AltitudeBiome(
      0.499f, 
      a
      );

  BiomeMaterial* water = new BiomeMaterial(
      0.0f,
      5.0f,
      glm::vec3(0.1f),
      glm::vec3(0.1f,0.1f,0.31f)*0.8f,
      glm::vec3(0.7)
      );
  std::vector<BiomeMaterial>* c = new std::vector<BiomeMaterial>();
  c->push_back(*water);
  AltitudeBiome* ocean = new AltitudeBiome(
      0.0f, 
      c
      );

  m_AltBiomes = new std::vector<AltitudeBiome>(); 
  m_AltBiomes->push_back(*ocean);
  m_AltBiomes->push_back(*valleys);
  m_AltBiomes->push_back(*tops);

}



