#ifndef Biome_H
#define Biome_H

#include "YGen.h"
#include "Material.h"
#define DEFAULT_MIN_HEIGHT      -4000
#define DEFAULT_MAX_HEIGHT      4000

class Biome{

  public:
    Biome(YGen* height, Material * m, int maxHeight = DEFAULT_MAX_HEIGHT, int minHeight = DEFAULT_MIN_HEIGHT) : m_Height(height), m_Material(m), m_MaxHeight(maxHeight), m_MinHeight(minHeight) {}
    YGen* m_Height;
    int m_MaxHeight, m_MinHeight;
    Material* m_Material;

    // getters
    int GetMaxHeight(){ return m_MaxHeight; }
    int GetMinHeight(){ return m_MinHeight; }

};

#endif
