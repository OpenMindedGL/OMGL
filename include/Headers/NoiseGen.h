#ifndef NoiseGen_H
#define NoiseGen_H
#include <vector>
#include <glm/glm.hpp>

#include "FastNoise.h"

#define MAX_OCTAVES 10

class NoiseGen {

  private :

    std::vector<FastNoise> m_Biomes;
    std::vector<FastNoise> simplex_fractal;
    std::vector<FastNoise> simplex_fractal2;
    std::vector<FastNoise> simplex_fractal3;
    
    long long int seed;
    unsigned char nbOctave;
    float lacunarity;
    float persistence;
    float zoom;
    float biomes_size_coef;
    unsigned char m_NbBiomes;

  public :

    NoiseGen();

    float compute(float x, float y);
    float compute_biome1(float x, float y);
    int compute_biome2(float x, float y);
    float compute_biome3(float x, float y);
    float compute_biome4(float x, float y);
    float compute_biome5(float x, float y);
    float compute_biome6(float x, float y);
    float compute_biome7(float x, float y);
    float compute_step(float x, float y);
    inline float compute(glm::vec2 a) { return compute(a.x, a.y); }

    inline float    GetLacunarity(){ return lacunarity; };
    inline float    GetPersistence(){ return persistence; };
    inline float    GetZoom(){ return zoom; };

    inline void    SetLacunarity(float a){  lacunarity= a; }; 
    inline void    SetPersistence(float a){  persistence= a; }; 
    inline void    SetZoom(float a){  zoom= a; }; 
};

#endif
