#ifndef DynamicHeightMap_H
#define DynamicHeightMap_H
#pragma once

#include <glm/glm.hpp>
#include <string.h>

#include "HeightMap.h"
#include "DynamicTexture.h"
#include "NoiseGen.h"

#define HEIGHT_MAP_EXCESS       2



class DynamicHeightMap : public HeightMap, public DynamicTexture
{

private: 

protected:


public:

  DynamicHeightMap( std::string path, glm::vec2 step = glm::vec2(1.0f,1.0f), glm::i32vec2 base = glm::i32vec2(0,0)) : NoiseTexture(), DynamicTexture(), HeightMap(path, step, base) {} 

  DynamicHeightMap( NoiseGen* n, unsigned int width, int texsize, glm::vec2 step = glm::vec2(1.0f,1.0f), glm::i32vec2 base = glm::i32vec2(0,0)) : NoiseTexture(n,width,texsize,step,base), DynamicTexture(), HeightMap(n,width,texsize,step,base){ m_TexelSize = texsize ;m_TorBegin = m_Base; m_TorBase = glm::i32vec2(0);}


  virtual void UpdateTexel(glm::i32vec2& p, glm::i32vec2& s, glm::i32vec2& t, std::vector<glm::u8vec4>& buffer);
};
#endif
