#ifndef DynamicBiomeHeightMap_H
#define DynamicBiomeHeightMap_H
#pragma once

#include <glm/glm.hpp>
#include <string.h>

#include "BiomeHeightMap.h"
#include "DynamicTexture.h"
#include "NoiseGen.h"

#define HEIGHT_MAP_EXCESS       2



class DynamicBiomeHeightMap : public BiomeHeightMap, public DynamicTexture
{

private: 

protected:

  DynamicBiomeHeightMap* m_Coarser = NULL;


public:

  //DynamicBiomeHeightMap( std::string path, glm::vec2 step = glm::vec2(1.0f,1.0f), glm::i32vec2 base = glm::i32vec2(0,0)) : NoiseTexture(), DynamicTexture(), BiomeHeightMap(path, step, base) {} 

  DynamicBiomeHeightMap( ProcMixer* n, unsigned int width, int texsize, glm::vec2 step = glm::vec2(1.0f,1.0f), glm::i32vec2 base = glm::i32vec2(0,0), DynamicBiomeHeightMap* coarser = NULL) : NoiseTexture(n,width,texsize,step,base), DynamicTexture(), BiomeHeightMap(n,width,texsize,step,base), m_Coarser(coarser){ m_TexelSize = texsize ;m_TorBegin = m_Base; m_TorBase = glm::i32vec2(0); m_Coarser = coarser;}


  virtual void UpdateTexel(glm::i32vec2& p, glm::i32vec2& s, glm::i32vec2& t, std::vector<glm::u8vec4>& buffer);
};
#endif
