#ifndef SKYBOX_H
#define SKYBOX_H

#include "Cubemap.h"
#include "Cube.h"

class Skybox : public Cube{

  private: 

//    Cubemap * m_Cubemap; 
  public:
    
    Skybox(std::string shaderPath);
};


#endif
