#ifndef SKYBOX_H
#define SKYBOX_H

#include "Cubemap.h"
#include "Cube.h"

class Skybox : public Cube{

  public:
    
    Skybox(std::string shaderPath = "shaders/Skybox.shader", std::vector<std::string> faces = {
        "textures/skybox_right.dds",
        "textures/skybox_left.dds",
        "textures/skybox_top.dds",
        "textures/skybox_bottom.dds",
        "textures/skybox_front.dds",
        "textures/skybox_back.dds"
        }
    );
};


#endif
