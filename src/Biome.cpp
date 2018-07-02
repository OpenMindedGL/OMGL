#include "Biome.h"




unsigned int BiomeMaterial::UniformSize = 5*sizeof(glm::vec4);

unsigned int AltitudeBiome::UniformSize = 4*sizeof(int) + BiomeMaterial::UniformSize * MAX_BIOME_MATS;

unsigned int Biome::UniformSize = 4*sizeof(int) + AltitudeBiome::UniformSize * MAX_ALT_BIOMES;



Biome::Biome(YGen* height, int maxHeight , int minHeight , std::vector<AltitudeBiome>* altBiomes) :
  m_Height(height),
  m_MaxHeight(maxHeight),
  m_MinHeight(minHeight),
  m_AltBiomes(altBiomes)
{

}

/* putting uniforms in a buffer, contiguously in memory
 * with everything correctly aligned for layout std140
 * (vectors 16 aligned, scalar 4 aligned)
 * the whole thing is messy, should have a bunch of structs */

void* Biome::FillUniformBuffer(void* buffer){
  int* p = (int*) buffer;
  float* p2 = (float*) buffer;
  p[0] = m_MinHeight;
  p[1] = m_MaxHeight;
  p[2] = m_AltBiomes->size();
  p2[3] = 0; 
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
  glm::vec4* p = (glm::vec4*) buffer;
  p[0] = glm::vec4(0);
  void* p1 = (void*) &p[1];
  for(int i = 0;i<MAX_ALT_BIOMES;i++){
    p1 = AltitudeBiome::FillEmptyUniform(p1);
  }
  return p1;
}

void* AltitudeBiome::FillEmptyUniform(void* buffer){
  glm::vec4* p = (glm::vec4*) buffer;
  p[0] = glm::vec4(0);
  void* p2 = (void*) &p[1];
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
  p[2] = m_NoiseRange;
  p[3] = m_NoiseFrequency;
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
  p[3] = m_NoiseRange;

  p[4] = m_NoiseFrequency;
  p[5] = GetNs();
  p[6] = 0;
  p[7] = 0;
  glm::vec4* p2 = (glm::vec4*) buffer;
  glm::vec3 a = GetKa();
  p2[2] = glm::vec4(a.x,a.y,a.z,0);
  a = GetKd();
  p2[3] = glm::vec4(a.x,a.y,a.z,0);
  a = GetKs();
  p2[4] = glm::vec4(a.x,a.y,a.z,0);
  
  return (void*) &p2[5];

}

void* BiomeMaterial::FillEmptyUniform(void* buffer){
  glm::vec4* p2 = (glm::vec4*) buffer;
  p2[0] = glm::vec4(0);
  p2[1] = glm::vec4(0);
  p2[2] = glm::vec4(0);
  p2[3] = glm::vec4(0);
  p2[4] = glm::vec4(0);
  return (void*) &p2[5];

}
