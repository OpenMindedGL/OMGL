#ifndef HeightMap_H
#define HeightMap_H
#pragma once

#include <glm/glm.hpp>
#include <string.h>

#include "Texture.h"
#include "NoiseGen.h"


#define HEIGHTMAP_SAMPLER_NAME    "u_HeightMap"
#define JPG_QUALITY    100

class HeightMap
{

private: 

protected:
  Texture* m_Texture;
  NoiseGen* m_Noise;
  std::vector<float> m_HeightsD;        // Decoded heights
  // TODO : remove (not worth keeping in memory)
  std::vector<glm::u8vec4> m_HeightsE;  // Encoded heights 
  unsigned int m_Width;
  glm::i32vec2 m_Base;  // bottom left in world coords
  glm::i32vec2 m_Step;  // bottom left in world coords

  void Encode();
  void Decode();
  void Gen(glm::i32vec2& base, glm::vec2& step);

  /* packing functions */
  float DecodeFloatRGBA( glm::vec4 rgba );
  glm::vec4 EncodeFloatRGBA( float v );

  /* pre-computed */
  static glm::vec4 packing0; 
  static glm::vec4 packing1; 
  static glm::vec4 packing2; 
  /*--------------*/

public:

  inline HeightMap( Texture* t, glm::vec2 step = glm::vec2(1.0f,1.0f), glm::i32vec2 base = glm::i32vec2(0,0)) : m_Texture(t), m_Width(t->GetWidth()), m_Step(step), m_Base(base) {} 

  HeightMap( NoiseGen* n, unsigned int width, glm::vec2 step = glm::vec2(1.0f,1.0f), glm::i32vec2 base = glm::i32vec2(0,0));

  Texture* MakeNormalMap();
  //void Load(std::string filepath);
  inline Texture* GetTexture() { return m_Texture; }



};
#endif
