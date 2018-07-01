#include "Biome.h"

unsigned int BiomeMaterial::UniformSize = 4*sizeof(glm::vec4);

unsigned int AltitudeBiome::UniformSize = 4*sizeof(int) + BiomeMaterial::UniformSize * MAX_BIOME_MATS;

unsigned int Biome::UniformSize = 4*sizeof(int) + AltitudeBiome::UniformSize * MAX_ALT_BIOMES;



Biome::Biome(YGen* height, int maxHeight , int minHeight , std::vector<AltitudeBiome>* altBiomes ) :
  m_Height(height),
  m_MaxHeight(maxHeight),
  m_MinHeight(minHeight),
  m_AltBiomes(altBiomes)
{

}

void* Biome::FillUniformBuffer(void* buffer){
  int* p = (int*) buffer;
  p[0] = m_MinHeight;
  p[1] = m_MaxHeight;
  p[2] = m_AltBiomes->size();
  p[3] = 0;
  void* p1 = (void*) &p[4];
  int i;
  for(i = 0;i<m_AltBiomes->size() ;i++){
    p1 = m_AltBiomes->at(i).FillUniformBuffer(p1);
  }
  for(;i<MAX_ALT_BIOMES;i++){
    p1 = AltitudeBiome::FillEmptyUniform(p1);
  }
  return p1;
}

void* Biome::FillEmptyUniform(void* buffer){
  int* p = (int*) buffer;
  p[0] = 0;
  p[1] = 0;
  p[2] = 0;
  p[3] = 0;
  void* p1 = (void*) &p[4];
  for(int i = 0;i<MAX_ALT_BIOMES;i++){
    p1 = AltitudeBiome::FillEmptyUniform(p1);
  }
  return p1;
}

void* AltitudeBiome::FillEmptyUniform(void* buffer){
  float* p = (float*) buffer;
  p[0] = 0;
  p[1] = 0;
  p[2] = 0;
  p[3] = 0;
  void* p2 = (void*) &p[4];
  for(int i = 0;i<MAX_BIOME_MATS;i++){
    p2 = BiomeMaterial::FillEmptyUniform(p2);
  }
  return p2;

}

void* AltitudeBiome::FillUniformBuffer(void* buffer){
  float* p = (float*) buffer;
  p[0] = m_MinAltitude;
  int* p1 = (int*) buffer;
  p1[1] = m_BiomeMaterials->size();
  p1[2] = 0;
  p1[3] = 0;
  void* p2 = (void*) &p[4];
  int i;
  for(i = 0;i<m_BiomeMaterials->size() ;i++){
    p2 = m_BiomeMaterials->at(i).FillUniformBuffer(p2);
  }
  for(;i<MAX_BIOME_MATS;i++){
    p2 = BiomeMaterial::FillEmptyUniform(p2);
  }
  return p2;

}

void* BiomeMaterial::FillUniformBuffer(void* buffer){
  float* p = (float*) buffer;
  p[0] = m_MinCost;
  int* p1 = (int*) buffer;
  p1[1] = m_Tex;
  p1[2] = m_TexSpread;
  p[3] = GetNs();
  glm::vec4* p2 = (glm::vec4*) buffer;
  glm::vec3 a = GetKa();
  p2[1] = glm::vec4(a.x,a.y,a.z,0);
  a = GetKd();
  p2[2] = glm::vec4(a.x,a.y,a.z,0);
  a = GetKs();
  p2[3] = glm::vec4(a.x,a.y,a.z,0);
  
  return (void*) &p2[4];

}

void* BiomeMaterial::FillEmptyUniform(void* buffer){
  glm::vec4* p2 = (glm::vec4*) buffer;
  p2[0] = glm::vec4(0);
  p2[1] = glm::vec4(0);
  p2[2] = glm::vec4(0);
  p2[3] = glm::vec4(0);
  return (void*) &p2[4];

}
