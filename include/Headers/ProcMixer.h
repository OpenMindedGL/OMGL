#ifndef ProcMixer_H
#define ProcMixer_H

#include "YGen.h"
#include "NoiseGen.h"
#include "FastNoise.h"
#include <vector>
#include <tuple>

#define DEFAULT_NOISES_SIZE     15.0f

class ProcMixer : public virtual YGen{

  protected:

  public:
    ProcMixer(){};
    ProcMixer(std::vector<YGen*>* noises) : m_Noises(noises){}
    std::vector<YGen*>* m_Noises;
    std::tuple<float, unsigned int> mix(float x, float y);
    inline unsigned int GetBiome(float x, float y) { return std::get<0>(mix(x,y)); }
    inline float compute(float x, float y){ return std::get<1>(mix(x,y)); }
};

class OMGLProcMixer : public ProcMixer, public NoiseGen {
  private:
    std::vector<FastNoise*>* m_Bits;
    float m_NoisesSize;

  public:
    OMGLProcMixer(std::vector<YGen*> * noises, float noisesSize = DEFAULT_NOISES_SIZE);
    std::tuple<float, unsigned int> mix(float x, float y);
    float compute(float x, float y);
    int sign(float x);


};
#endif
