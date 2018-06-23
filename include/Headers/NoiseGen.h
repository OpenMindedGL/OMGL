#ifndef NoiseGen_H
#define NoiseGen_H
#include <vector>
#include <glm/glm.hpp>

#include "FastNoise.h"
#include "YGen.h"
#include "Biome.h"

#define MAX_OCTAVES 10

class NoiseGen : public virtual YGen {

  protected :

    std::vector<FastNoise> simplex_fractal;
    std::vector<FastNoise> simplex_fractal2;
    std::vector<FastNoise> simplex_fractal3;
    
    long long int seed;
    unsigned char nbOctave;
    float lacunarity;
    float persistence;
    float zoom;
    float biomes_size_coef;

    float compute1(float x, float y);
    float compute4(float x, float y);

  public :

    NoiseGen();
    float compute(float x, float y) = 0;

    inline void    SetLacunarity(float a){  lacunarity= a; }; 
    inline void    SetPersistence(float a){  persistence= a; }; 
    inline void    SetZoom(float a){  zoom= a; }; 
};

class NoiseCloudVerso : public NoiseGen{
  private:
    std::vector<FastNoise> m_Biomes;
  public:
    NoiseCloudVerso(): NoiseGen(){
      for(unsigned int l = 0; l < 3; l++){
        FastNoise temp_noise(seed+l+(nbOctave*4));
        temp_noise.SetNoiseType(FastNoise::SimplexFractal);
        temp_noise.SetFractalOctaves(8);
        temp_noise.SetFrequency(lacunarity * 0.01f * (1.0f/biomes_size_coef));
        m_Biomes.push_back(temp_noise);
      }
    };
    float compute(float x, float y);
};

class NoiseCloudRecto : public NoiseGen{
  private:
    std::vector<FastNoise> m_Biomes;
  public:
    NoiseCloudRecto(): NoiseGen(){
      for(unsigned int l = 0; l < 3; l++){
        FastNoise temp_noise(seed+l+(nbOctave*4));
        temp_noise.SetNoiseType(FastNoise::SimplexFractal);
        temp_noise.SetFractalOctaves(8);
        temp_noise.SetFrequency(lacunarity * 0.01f * (1.0f/biomes_size_coef));
        m_Biomes.push_back(temp_noise);
      }
    };
    float compute(float x, float y);
};

class NoiseBiome1 : public NoiseGen{
  public:
    float compute(float x, float y);
};

class NoiseBiome2 : public NoiseGen{
  public:
    float compute(float x, float y);
};

class NoiseBiome3 : public NoiseGen{
  public:
    float compute(float x, float y);
};

class NoiseBiome4 : public NoiseGen{
  public:
    float compute(float x, float y);
};

class NoiseBiome5 : public NoiseGen{
  public:
    float compute(float x, float y);
};

class NoiseBiome6 : public NoiseGen{
  public:
    float compute(float x, float y);
};

class NoiseBiome7 : public NoiseGen{
  public:
    float compute(float x, float y);
};

class NoiseMixBiome : public NoiseGen{
  private:
    std::vector<Biome> myBiomes;
    std::vector<FastNoise> m_Biomes;

  public:
    NoiseMixBiome(std::vector<Biome> Biomes): NoiseGen(){
      for (std::vector<Biome>::iterator my_it= Biomes.begin(); my_it < Biomes.end(); my_it++)
        myBiomes.push_back(*my_it);
      for(unsigned int l = 1; glm::pow(2,l) <= Biomes.size(); l++){
        FastNoise temp_noise(seed+l+(nbOctave*4));
        temp_noise.SetNoiseType(FastNoise::SimplexFractal);
        temp_noise.SetFractalOctaves(8);
        temp_noise.SetFrequency(lacunarity * 0.01f * (1.0f/biomes_size_coef));
        m_Biomes.push_back(temp_noise);
      }
    };
    float compute(float x, float y);
    int sign(float x);

};

#endif
