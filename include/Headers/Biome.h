#ifndef Biome_H
#define Biome_H

#include "YGen.h"

class Biome{

  public:
    Biome(YGen* height) : m_Height(height) {}
    YGen* m_Height;

};

#endif
