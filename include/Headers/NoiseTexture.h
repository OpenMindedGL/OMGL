#ifndef NoiseTexture_H
#define NoiseTexture_H
#pragma once

#include "NoiseGen.h"
#include "Texture.h"
#include <string>
#include <vector>


class NoiseTexture : public Texture
{
  protected :

    YGen* m_Noise;
    glm::vec2 m_Step;  // bottom left in world coords
    std::vector<glm::u8vec4> m_Texels;  
    int m_TexelSize;
    glm::i32vec2 m_Base;  // bottom left in world coords

  public: 
    virtual void Gen(glm::i32vec2& base, glm::vec2& step){}

  
    NoiseTexture( std::string path, glm::vec2 step = glm::vec2(1.0f,1.0f), glm::i32vec2 base = glm::i32vec2(0,0)) : Texture(path,DEFAULT_SAMPLER_NAME, 0, false), m_Step(step), m_Base(base) {} 

    NoiseTexture( YGen* n, unsigned int width, int texsize, glm::vec2 step = glm::vec2(1.0f,1.0f), glm::i32vec2 base = glm::i32vec2(0,0)) : Texture(width), m_Noise(n), m_TexelSize(texsize), m_Base(base), m_Step(step) {}

    NoiseTexture(){}

    // getters
    inline glm::i32vec2 GetBase() { return m_Base; }
    inline int GetTexelSize() { return m_TexelSize; }
    
    // setters
    inline void SetBase(glm::i32vec2 b){ m_Base = b; };
    inline void SetStep(glm::i32vec2 s){ m_Step = s; };

};

#endif
