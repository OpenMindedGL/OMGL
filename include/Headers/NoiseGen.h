#ifndef NoiseGen_H
#define NoiseGen_H
#include <vector>
#include <glm/glm.hpp>

#include "FastNoise.h"

#define MAX_OCTAVES 10

class NoiseGen {

  private :

    std::vector<FastNoise> simplex_fractal; //mountains
    FastNoise simplex;                      //flat land
    FastNoise mix;                          //mix factor
    
    long long int seed;
    unsigned char nbOctave;
    float lacunarity;
    float persistence;
    float zoom;
    float mix_freq;
    float simplex_freq;

    long int * m_PerlinPerm2DTable;  
    long int * m_PerlinGrad2DTable;  


  public :

    NoiseGen();

    float compute(float x, float y);
    glm::vec3 noised( glm::vec2 p, float seed2 );
    void createPerlinPerm2DTable(unsigned char (&perlinPermTable)[256]);
    void createPerlinGrad2DTable(unsigned char (&perlinPermTable)[256], unsigned char (&perlinGradTable)[16]);
    inline float compute(glm::vec2 a) { return compute(a.x, a.y); }

    inline float    GetLacunarity(){ return lacunarity; };
    inline float    GetPersistence(){ return persistence; };
    inline float    GetZoom(){ return zoom; };
    inline float    GetMix_freq(){ return mix_freq; };
    inline float    GetSimplex_freq(){ return simplex_freq; };

    inline void    SetLacunarity(float a){  lacunarity= a; }; 
    inline void    SetPersistence(float a){  persistence= a; }; 
    inline void    SetZoom(float a){  zoom= a; }; 
    inline void    SetMix_freq(float a){  mix_freq= a;}; 
    inline void    SetSimplex_freq(float a){  simplex_freq= a; }; 
};

#endif
