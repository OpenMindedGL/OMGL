#ifndef DynamicHeightMap_H
#define DynamicHeightMap_H
#pragma once

#include <glm/glm.hpp>
#include <string.h>

#include "HeightMap.h"


#define JPG_QUALITY    100

class DynamicHeightMap : public HeightMap
{
private: 

  int GetIndex(glm::i32vec2& p);
public:

  void Upload(glm::i32vec2 offset, glm::i32vec2 size, const void* data);
  unsigned int Update(glm::i32vec2 dir);

  void UpdateSub(glm::i32vec2& s, glm::i32vec2& e);

  void UpdateTexel(glm::i32vec2& p, glm::i32vec2& s,std::vector<float>& buffer);



};
#endif
