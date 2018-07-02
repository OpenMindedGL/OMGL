#ifndef OMGLNoiseGen_H
#define OMGLNoiseGen_H

#include <vector>

#include "FastNoise.h"
#include "NoiseGen.h"

#define DEFAULT_CLOUD_SIZE      15.0f

class OMGLNoiseGen : public virtual NoiseGen {
  protected:
    std::vector<std::vector<FastNoise*>> m_Noises;
  
  public:
    OMGLNoiseGen(
        unsigned int nbNoises = DEFAULT_NB_NOISES,
        long long int seed = DEFAULT_SEED,
        unsigned char nbOctaves = DEFAULT_NB_OCTAVES,
        float lacunarity = DEFAULT_LACUNARITY,
        float persistence = DEFAULT_PERSISTENCE,
        float zoom = DEFAULT_ZOOM
        );
        

};

class OMGLBiomeMatMixer: public OMGLNoiseGen {
  public:
    OMGLBiomeMatMixer() : OMGLNoiseGen(1,6969) {}
    float compute(float x, float y);
};

class OMGLNoiseCloud : public virtual OMGLNoiseGen{
  protected:
    FastNoise* m_Mix;
    float m_CloudSize = DEFAULT_CLOUD_SIZE;
    float compute(float x, float y) { return compute2(x,y)[0]; }
    float* compute2(float x, float y);

  public:
    OMGLNoiseCloud();

};

typedef OMGLNoiseCloud OMGLNoiseCloudVerso;

class OMGLNoiseCloudRecto : public OMGLNoiseCloud{
  public:
    OMGLNoiseCloudRecto() : OMGLNoiseCloud() {};
    float compute(float x, float y);
};

class OMGLNoiseBiome : public virtual OMGLNoiseGen{
  protected:
    virtual float compute(float x, float y){return 0.0f;}
    float compute1(float x, float y);
    float compute4(float x, float y);

  public:
    OMGLNoiseBiome() : OMGLNoiseGen(3) {}

};

class OMGLNoiseBiome1 : public OMGLNoiseBiome{
  public:
    float compute(float x, float y);
};

class OMGLNoiseBiome2 : public OMGLNoiseBiome{
  public:
    float compute(float x, float y);
};

class OMGLNoiseBiome3 : public OMGLNoiseBiome{
  public:
    float compute(float x, float y);
};

class OMGLNoiseBiome4 : public OMGLNoiseBiome{
  public:
    float compute(float x, float y);
};

class OMGLNoiseBiome5 : public OMGLNoiseBiome{
  public:
    float compute(float x, float y);
};

class OMGLNoiseBiome6 : public OMGLNoiseBiome{
  public:
    float compute(float x, float y);
};

class OMGLNoiseBiome7 : public OMGLNoiseBiome{
  public:
    float compute(float x, float y);
};

#endif 
