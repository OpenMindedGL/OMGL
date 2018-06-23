#ifndef Sampler_H
#define Sampler_H
#pragma once

#include "Shader.h"
#include "Texture.h"
#include <string>
#include <vector>

/* Samplers allow the use of the same texture with different parameters */
/* TODO : all functions relative to loading should be overloaded empty
 * or they should be isolated from Texture */
class Sampler : public Texture
{
  protected :

  public: 

  
    Sampler(std::string name, unsigned char slot, unsigned int interp = DEFAULT_INTERP_VALUE, unsigned int wrap = DEFAULT_WRAP_VALUE);

    inline Sampler(Texture* t, std::string name, unsigned int interp = DEFAULT_INTERP_VALUE, unsigned int wrap = DEFAULT_WRAP_VALUE) : 
      Sampler(name,t->GetSlot(),interp,wrap) {}

    Sampler(){}
    ~Sampler();

    void Bind(unsigned int slot) const; 
    inline void Bind() const { Bind(m_Slot); }

    // getters
    
    // setters
    inline void SetName(std::string n){ m_Name = n; };
    inline void SetSlot(unsigned char s){ m_Slot = s; };

};

#endif
