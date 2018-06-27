#ifndef HeightMap_H
#define HeightMap_H
#pragma once

#include <glm/glm.hpp>
#include <string.h>

#include "NoiseTexture.h"
#include "NoiseGen.h"


#define HEIGHTMAP_SAMPLER_NAME    "u_HeightMap"
#define JPG_QUALITY    100

class HeightMap : virtual public NoiseTexture
{

private: 

protected:

  std::vector<float> m_HeightsD;        // Decoded heights

  void Encode();
  void Decode();
  virtual void Gen(glm::i32vec2& base, glm::vec2& step);

  /* packing functions */
  float DecodeFloatRGBA( glm::vec4 rgba );
  glm::vec4 EncodeFloatRGBA( float v );

  /* pre-computed */
  static glm::vec4 packing0; 
  static glm::vec4 packing1; 
  static glm::vec4 packing2; 
  /*--------------*/

public:

  HeightMap( std::string path, glm::vec2 step = glm::vec2(1.0f,1.0f), glm::i32vec2 base = glm::i32vec2(0,0)) : NoiseTexture(path, step, base) {} 

  HeightMap( NoiseGen* n, unsigned int width, int texsize, glm::vec2 step = glm::vec2(1.0f,1.0f), glm::i32vec2 base = glm::i32vec2(0,0));

  //Texture* MakeNormalMap();
  float GetHeight(glm::i32vec2 p) { return m_HeightsD[p.x+p.y*m_Width]; }
  float ComputeHeight(glm::i32vec2 p);
  //void Load(std::string filepath);



};
#endif
