#include <vector>
#include <glm/glm.hpp>

#include "FastNoise.h"

#define MAX_OCTAVES 10

class NoiseGen {

  private :

    std::vector<FastNoise> simplex_fractal;
    FastNoise simplex;
    FastNoise mix;
    
    long long int seed;
    float lacunarity;
    float persistence;
    float zoom;
    float mix_freq;
    float simplex_freq;

  public :

    NoiseGen();
    ~NoiseGen();

    float compute(float x, float y);
    inline float compute(glm::vec2 a) { return compute(a.x, a.y); }
}

