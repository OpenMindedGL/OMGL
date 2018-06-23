#ifndef DynamicTexture_H
#define DynamicTexture_H
#pragma once

#include <glm/glm.hpp>
#include <string.h>

#include "NoiseTexture.h"


class DynamicTexture : virtual public NoiseTexture
{
protected: 

  int GetIndex(glm::i32vec2& p);
  glm::i32vec2 m_TorBegin;
  glm::i32vec2 m_TorBase;

public:
  glm::i32vec2 GetTorPos(glm::i32vec2& p);

  DynamicTexture() : m_TorBegin(glm::i32vec2(0)){}

  void Upload(glm::i32vec2 offset, glm::i32vec2 size, const void* data);
  unsigned int Update(glm::i32vec2 dir);

  void UpdateSub(glm::i32vec2& s, glm::i32vec2& e);

  virtual void UpdateTexel(glm::i32vec2& p, glm::i32vec2& s, glm::i32vec2& t, std::vector<glm::u8vec4>& buffer);

  // Getters
  inline glm::i32vec2 GetTorBegin() { return m_TorBegin; }
  inline glm::i32vec2 GetTorBase() { return m_TorBase; }




};
#endif
