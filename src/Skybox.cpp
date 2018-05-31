#include "Skybox.h"
#include "Cube.h"

#include <string.h>

Skybox::Skybox(std::string shaderPath, std::vector<std::string> faces) : Object(new Cube(), shaderPath)
{
  this->GetMaterials().at(0)->SetTexture(new Cubemap(faces));
  this->GetMaterials().at(0)->InitTexture("u_SkyboxSampler", 0);
}

