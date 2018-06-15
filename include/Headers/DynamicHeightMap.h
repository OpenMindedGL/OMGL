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

public:

  void Upload(glm::i32vec2 offset, glm::i32vec2 size);
  void Update(glm::vec2 center);



};
#endif
