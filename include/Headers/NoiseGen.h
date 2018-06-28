#ifndef NoiseGen_H
#define NoiseGen_H

#include <vector>

#include "YGen.h"

#define DEFAULT_NB_NOISES         3
#define DEFAULT_NB_OCTAVES        8
#define DEFAULT_LACUNARITY        2.0f
#define DEFAULT_PERSISTENCE       0.5f
#define DEFAULT_ZOOM              3.0f
#define DEFAULT_SEED              62
#define MAX_OCTAVES 10

class NoiseGen : public virtual YGen {

  protected :
    
    long long int m_Seed;
    unsigned char m_Octaves;
    float m_Lacunarity;
    float m_Persistence;
    float m_Zoom;


  public :

    NoiseGen(
        long long int seed = DEFAULT_SEED,
        unsigned char nbOctaves = DEFAULT_NB_OCTAVES,
        float lacunarity = DEFAULT_LACUNARITY,
        float persistence = DEFAULT_PERSISTENCE,
        float zoom = DEFAULT_ZOOM
        ) :
      m_Seed(seed),
      m_Octaves(nbOctaves),
      m_Lacunarity(lacunarity),
      m_Persistence(persistence),
      m_Zoom(zoom) {}

    float compute(float x, float y) = 0;

    inline void    SetLacunarity(float a){  m_Lacunarity= a; }; 
    inline void    SetPersistence(float a){  m_Persistence= a; }; 
    inline void    SetZoom(float a){  m_Zoom= a; }; 
};



#endif
