#include "Skybox.h"
#include "Cube.h"

#include <string.h>

Skybox::Skybox(std::string shaderPath) : Cube(shaderPath)
{
  m_Texture = new Cubemap();
  InitTexture("u_SkyboxSampler", 0);
  Scale(glm::vec3(100,100,100));
}
