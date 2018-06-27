#ifndef OMGLBiomes_H
#define OMGLBiomes_H

#include "Biome.h"
#include "OMGLNoiseGen.h"

class OMGLMountains : public Biome{

  public:
    OMGLMountains() : Biome(new OMGLNoiseBiome1(),500,-500) {}

};

class OMGLDesert : public Biome{

  public:
    OMGLDesert() : Biome(new OMGLNoiseBiome3(), 500, -500) {}

};
class OMGLBlocky : public Biome{

  public:
    OMGLBlocky() : Biome(new OMGLNoiseBiome2(), 200, 0) {}

};
class OMGLPeanuts : public Biome{

  public:
    OMGLPeanuts() : Biome(new OMGLNoiseBiome4(), 200, 0) {}

};
#endif

