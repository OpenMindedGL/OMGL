#include "Skybox.h"
#include "Cube.h"

#include <string.h>

Skybox::Skybox(std::string shaderPath, std::vector<std::string> faces) : Cube(shaderPath)
{
  m_Texture = new Cubemap(faces);
  InitTexture("u_SkyboxSampler", 0);
}

