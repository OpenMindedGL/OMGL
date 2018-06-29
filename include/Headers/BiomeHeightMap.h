#ifndef BiomeHeightMap_H
#define BiomeHeightMap_H
#pragma once

#include <glm/glm.hpp>
#include <string.h>
#include <tuple>

#include "HeightMap.h"
#include "ProcMixer.h"

class BiomeHeightMap : virtual public HeightMap
{

private: 

  ProcMixer* m_Noise;

protected:

  std::vector<unsigned char> m_Biomes;        // Decoded heights

  void Encode();
  void Decode();
  virtual void Gen(glm::i32vec2& base, glm::vec2& step);

  /* packing functions */
  float DecodeFloatRGB( glm::vec3 rgb );
  glm::vec3 EncodeFloatRGB( float v);

  /* pre-computed */
  static glm::vec4 packing0; 
  static glm::vec4 packing1; 
  static glm::vec4 packing2; 
  /*--------------*/

public:


  BiomeHeightMap( ProcMixer* n, unsigned int width, int texsize, glm::vec2 step = glm::vec2(1.0f,1.0f), glm::i32vec2 base = glm::i32vec2(0,0));

  //Texture* MakeNormalMap();
  float GetHeight(glm::i32vec2 p) { return m_HeightsD[p.x+p.y*m_Width]; }
  float GetBiome(glm::i32vec2 p) { return m_Biomes[p.x+p.y*m_Width]; }
  std::tuple<float,unsigned int> ComputeHeightAndBiome(glm::i32vec2 p);
  //void Load(std::string filepath);



};
#endif
