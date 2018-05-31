#ifndef NoiseGen_H
#define NoiseGen_H
#include <vector>
#include <glm/glm.hpp>

#include "FastNoise.h"

#define MAX_OCTAVES 10

class NoiseGen {

  private :

    std::vector<FastNoise> simplex_fractal;
    std::vector<FastNoise> simplex_fractal2;
    std::vector<FastNoise> simplex_fractal3;
    
    long long int seed;
    unsigned char nbOctave;
    float lacunarity;
    float persistence;
    float zoom;

  public :

    NoiseGen();

    float compute(float x, float y);
    inline float compute(glm::vec2 a) { return compute(a.x, a.y); }

    inline float    GetLacunarity(){ return lacunarity; };
    inline float    GetPersistence(){ return persistence; };
    inline float    GetZoom(){ return zoom; };

    inline void    SetLacunarity(float a){  lacunarity= a; }; 
    inline void    SetPersistence(float a){  persistence= a; }; 
    inline void    SetZoom(float a){  zoom= a; }; 
};

#endif
