#include "OMGLBiomes.h"
OMGLMountains::OMGLMountains()  : 
  Biome(new OMGLNoiseBiome1(), 1000,-1000)
{
  BiomeMaterial* rock = new BiomeMaterial(
      0.6f,
      1.0f,
      glm::vec3(0.1f),
      glm::vec3(0.3f)*0.5f,
      glm::vec3(0.4f),
      0.01,
      0.05
      );
  BiomeMaterial* snow = new BiomeMaterial(
      0.0f,
      3.5f,
      glm::vec3(0.1f),
      glm::vec3(0.7f),
      glm::vec3(0.9f),
      0.01,
      0.05
      );
  std::vector<BiomeMaterial>* b = new std::vector<BiomeMaterial>();
  b->push_back(*snow);
  b->push_back(*rock);
  AltitudeBiome* tops = new AltitudeBiome(
      0.53, 
      b,
      0.01,
      0.05
      );


  BiomeMaterial* dirt = new BiomeMaterial(
      0.4f,
      1.0f,
      glm::vec3(0.1f),
      glm::vec3(0.3f,0.22f,0)*0.6f,
      glm::vec3(0),
      0.01,
      0.05
      );
  BiomeMaterial* grass = new BiomeMaterial(
      0.0f,
      1.0f,
      glm::vec3(0.1f),
      glm::vec3(0.1f,0.31f,0.1f)*0.4f,
      glm::vec3(0.0),
      0.01,
      0.05
      );
  std::vector<BiomeMaterial>* a = new std::vector<BiomeMaterial>();
  a->push_back(*grass);
  a->push_back(*dirt);
  AltitudeBiome* valleys = new AltitudeBiome(
      0.5003f, 
      a,
      0.0001,
      0.2
      );

  BiomeMaterial* sand = new BiomeMaterial(
      0.001f,
      1.0f,
      glm::vec3(0.2f),
      glm::vec3(0.8f,0.6,0.25f)*0.8f,
      glm::vec3(0)*1.6f,
      0.00,
      0.00
      );
  std::vector<BiomeMaterial>* d = new std::vector<BiomeMaterial>();
  d->push_back(*sand);
  AltitudeBiome* beach = new AltitudeBiome(
      0.50005f, 
      d,
      0.000001,
      1
      );

  BiomeMaterial* water = new BiomeMaterial(
      0.001f,
      2.0f,
      glm::vec3(0.1f),
      glm::vec3(0.1f,0.1f,0.31f)*0.8f,
      glm::vec3(0.1f,0.1f,0.31f)*1.5f,
      0.00,
      0.00
      );
  std::vector<BiomeMaterial>* c = new std::vector<BiomeMaterial>();
  c->push_back(*water);
  AltitudeBiome* ocean = new AltitudeBiome(
      0.001f, 
      c,
      0.0,
      0.0
      );

  m_AltBiomes = new std::vector<AltitudeBiome>(); 
  m_AltBiomes->push_back(*ocean);
  m_AltBiomes->push_back(*beach);
  m_AltBiomes->push_back(*valleys);
  m_AltBiomes->push_back(*tops);

}


OMGLDesert::OMGLDesert() : Biome(new OMGLNoiseBiome3(), 500, -500) {

  BiomeMaterial* sand = new BiomeMaterial(
      0.001f,
      1.5f,
      glm::vec3(0.5f),
      glm::vec3(0.3f,0.3f,0.1f),
      glm::vec3(0.1f),
      0.01,
      0.05
      );
  std::vector<BiomeMaterial>* a = new std::vector<BiomeMaterial>();
  a->push_back(*sand);
  AltitudeBiome* def = new AltitudeBiome(
      0.001f, 
      a
      );
  m_AltBiomes = new std::vector<AltitudeBiome>(); 
  m_AltBiomes->push_back(*def);

}

