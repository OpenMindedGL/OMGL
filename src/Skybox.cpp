#include "Skybox.h"
#include "Cubemap.h"
#include "Cube.h"

#include <string.h>

Skybox::Skybox(std::string shaderPath, std::vector<std::string> faces) : Object(new Cube(), new Material(new Cubemap(faces),new Shader(shaderPath))){}

