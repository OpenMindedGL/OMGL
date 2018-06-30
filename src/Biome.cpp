#include "Biome.h"

void* Biome::FillUniformBuffer(void* buffer){
  int* p = (int*) buffer;
  p[0] = m_MinHeight;
  p[1] = m_MaxHeight;
  p[2] = m_AltBiomes->size();
  p[3] = 0;
}

