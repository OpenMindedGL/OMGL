#ifndef Biome_H
#define Biome_H

#include <vector>

#include "YGen.h"
#include "Material.h"
#define DEFAULT_MIN_HEIGHT      -4000
#define DEFAULT_MAX_HEIGHT      4000

class BiomeMaterial : public Material {
  private:
    float m_MinCost;
    int m_TexSpread;
  public:
    void* FillUniformBuffer(void* buffer);
    BiomeMaterial(float mincost, Texture* t, int texspread) :
      Material(t),
      m_MinCost(mincost),
      m_TexSpread(texspread) {}
};

class AltitudeBiome {
  private:
    float m_MinAltitude;
    int m_NbBiomeMaterials;
    BiomeMaterial* m_BiomeMaterials;
  public:
    void* FillUniformBuffer(void* buffer);
    AltitudeBiome(float minalt, int nbmats, BiomeMaterial* biomemats) :
      m_MinAltitude(minalt),
      m_NbBiomeMaterials(nbmats),
      m_BiomeMaterials(biomemats) {}

};

class Biome{

  public:
    Biome(YGen* height, int maxHeight = DEFAULT_MAX_HEIGHT, int minHeight = DEFAULT_MIN_HEIGHT, std::vector<AltitudeBiome>* altBiomes = new std::vector<AltitudeBiome>() ) : m_Height(height), m_MaxHeight(maxHeight), m_MinHeight(minHeight), m_AltBiomes(altBiomes) {}
    YGen* m_Height;
    int m_MaxHeight, m_MinHeight;
    std::vector<AltitudeBiome>* m_AltBiomes;

    void* FillUniformBuffer(void* buffer);
    // getters
    int GetMaxHeight(){ return m_MaxHeight; }
    int GetMinHeight(){ return m_MinHeight; }

};

/*struct BiomeAligned{
  int minh;
  int maxh;
  int nb;
  int PADDING;
  struct AltBiomeAligned d[nb];
};*/



/*struct BiomeMatAligned {
  float mincos;
  int tex;
  int texs;
  float ns;
  glm::vec4 Ka;
  glm::vec4 Kd;
  glm::vec4 Ks;
};*/


/*struct AltBiomeAligned {
  float minalt;
  int nb;
  glm::vec2 PADDING;
  struct BiomeMatAligned[nb] biomemats;
};*/

#endif
