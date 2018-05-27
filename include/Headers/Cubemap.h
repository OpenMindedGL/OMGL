#ifndef CUBEMAP_H
#define CUBEMAP_H
#pragma once

#include <string>
#include <vector>

#include "Texture.h"

#define NB_FACES        6

class Cubemap : public Texture {

  private :

    static const unsigned int m_Target = GL_TEXTURE_CUBE_MAP;
    std::vector<std::string> m_Faces;

    void LoadFaces();
  public : 

    Cubemap(std::vector<std::string> faces);
};

#endif
