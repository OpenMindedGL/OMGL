#ifndef HeightMap_H
#define HeightMap_H
#pragma once

#include <glm/glm.hpp>

#include "Texture.h"
#include "NoiseGen.h"


#define HEIGHTMAP_SAMPLER_NAME    "u_HeightMap"

class HeightMap
{
private: 
  Texture* m_Texture;
  NoiseGen* m_Noise;
  std::vector<float> m_HeightsD;
  std::vector<glm::u8vec4> m_HeightsE;
  unsigned int m_Width;

  /* pre-computed */
  static glm::vec4 packing0; 
  static glm::vec4 packing1; 
  static glm::vec4 packing2; 
  /*--------------*/

public:

  inline HeightMap( Texture* t ) : m_Texture(t) {}
  HeightMap( NoiseGen* n, unsigned int width );
  HeightMap(float* buffer, unsigned int width);
  void Encode();
  void Decode();
  void Gen();
  float DecodeFloatRGBA( glm::vec4 rgba );
  glm::vec4 EncodeFloatRGBA( float v );
  inline Texture* GetTexture() { return m_Texture; }




};
#endif
