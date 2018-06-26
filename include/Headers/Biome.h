#ifndef Biome_H
#define Biome_H

#include "YGen.h"
#define DEFAULT_MIN_HEIGHT      -4000
#define DEFAULT_MAX_HEIGHT      4000

class Biome{

  public:
    Biome(YGen* height, int maxHeight = DEFAULT_MAX_HEIGHT, int minHeight = DEFAULT_MIN_HEIGHT) : m_Height(height), m_MaxHeight(maxHeight), m_MinHeight(minHeight) {}
    YGen* m_Height;
    int m_MaxHeight, m_MinHeight;

    // getters
    int GetMaxHeight(){ return m_MaxHeight; }
    int GetMinHeight(){ return m_MinHeight; }

};

#endif
