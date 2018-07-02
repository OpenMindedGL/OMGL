#ifndef Sky_H
#define Sky_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Object.h"
#include "YGen.h"
#include "OMGLNoiseGen.h"
#include "SkyMesh.h"

#define DEFAULT_SKY_SHADER "shaders/Clouds.shader"

class Sky : public Object
{

  public: 

    //Sky(glm::vec2 center) : Sky(center, new OMGLNoiseCloudRecto(), new OMGLNoiseCloudVerso()) {  }

    Sky(glm::vec2 center, YGen* rectoNoise, YGen* versoNoise)
      : Object(new SkyMesh(center, rectoNoise, versoNoise), new Material(new Shader(DEFAULT_SKY_SHADER))) {}

};

#endif
