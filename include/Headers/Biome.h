#ifndef Biome_H
#define Biome_H

#include <vector>

#include "YGen.h"
#include "Material.h"
#define DEFAULT_MIN_HEIGHT      -4000
#define DEFAULT_MAX_HEIGHT      4000
#define MAX_ALT_BIOMES  5
#define MAX_BIOME_MATS  3
#define MAX_BIOMES  20

class BiomeMaterial : public Material {
  private:
    float m_MinCost;
    int m_Tex;
    int m_TexSpread;
  public:
    static void* FillEmptyUniform(void* buffer);
    void* FillUniformBuffer(void* buffer);
    BiomeMaterial(
        float mincost,
        float Ns = 1,
        glm::vec3 Ka = glm::vec3(1),
        glm::vec3 Kd = glm::vec3(1),
        glm::vec3 Ks = glm::vec3(1),
        Texture* t = NULL,
        int texspread = 1
        ) :
      Material(t),
      m_MinCost(mincost),
      m_TexSpread(texspread)
  {
    SetNs(Ns);
    SetKa(Ka);
    SetKd(Kd);
    SetKs(Ks);
  }

    static unsigned int UniformSize;
};

class AltitudeBiome {
  private:
    float m_MinAltitude;
    std::vector<BiomeMaterial>* m_BiomeMaterials;
  public:
    static void* FillEmptyUniform(void* buffer);
    void* FillUniformBuffer(void* buffer);
    AltitudeBiome(float minalt,std::vector<BiomeMaterial>* biomemats) :
      m_MinAltitude(minalt),
      m_BiomeMaterials(biomemats) {}

    static unsigned int UniformSize;
};

class Biome{

  public:
    Biome(
        YGen* height,
        int maxHeight = DEFAULT_MAX_HEIGHT,
        int minHeight = DEFAULT_MIN_HEIGHT,
        std::vector<AltitudeBiome>* altBiomes = new std::vector<AltitudeBiome>()
        );
    YGen* m_Height;
    int m_MaxHeight, m_MinHeight;
    std::vector<AltitudeBiome>* m_AltBiomes;

    static unsigned int UniformSize;

    static void* FillEmptyUniform(void* buffer);
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
