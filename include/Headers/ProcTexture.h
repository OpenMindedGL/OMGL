#ifndef ProcTexture_H
#define ProcTexture_H
#pragma once

#include "NoiseGen.h"
#include "NoiseTexture.h"
#include <string>
#include <vector>


class ProcTexture : public NoiseTexture
{
  protected :
    glm::vec4 m_Color;

  public: 
    virtual void Gen(glm::i32vec2& base, glm::vec2& step);

  
    ProcTexture( YGen* n, unsigned int width, glm::vec2 step = glm::vec2(1.0f,1.0f), glm::i32vec2 base = glm::i32vec2(0,0)); 
    ProcTexture( std::string path, glm::vec2 step = glm::vec2(1.0f,1.0f), glm::i32vec2 base = glm::i32vec2(0,0)) : NoiseTexture(path, step, base) {} 
    ~ProcTexture();
    ProcTexture(){}
    ProcTexture(std::string path) : NoiseTexture(path) {}

    // getters
    
    // setters
    inline void SetBase(glm::i32vec2 b){ m_Base = b; };
    inline void SetStep(glm::i32vec2 s){ m_Step = s; };

};

#endif
