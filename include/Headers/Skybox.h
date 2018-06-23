#ifndef SKYBOX_H
#define SKYBOX_H

#include "Cubemap.h"
#include "Object.h"
#include "Cube.h"

class Skybox : public Object{

  public:
    
    Skybox(std::string shaderPath = "shaders/Skybox.shader", std::vector<std::string> faces = {
        "textures/skybox_right.jpg",
        "textures/skybox_left.jpg",
        "textures/skybox_top.jpg",
        "textures/skybox_bottom.jpg",
        "textures/skybox_front.jpg",
        "textures/skybox_back.jpg"
        }
    );
};

#endif
