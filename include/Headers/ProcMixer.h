#ifndef ProcMixer_H
#define ProcMixer_H

#include <vector>
#include <tuple>


class ProcMixer : public YGen{

  public:
    ProcMixer(){};
    ProcMixer(std::vector<YGen*>* noises) : m_Noises(noises){};
    std::vector<YGen*>* m_Noises;
    std::tuple<float, unsigned int> mix(float x, float y);
};

class OMGLProcMixer : public ProcMixer {
  private:
    std::vector<FastNoise*>* m_Bits;

  public:
    NoiseMixBiome(std::vector<YGen*>* noises): ProcMixer(noises) {
      FastNoise* temp_noise;
      for(unsigned int l = 1; glm::pow(2,l) <= m_Noises.size(); l++){
        temp_noise = new FastNoise(seed+l+(nbOctave*4));
        temp_noise->SetNoiseType(FastNoise::SimplexFractal);
        temp_noise->SetFractalOctaves(8);
        temp_noise->SetFrequency(lacunarity * 0.01f * (1.0f/biomes_size_coef));
        m_Bits->push_back(temp_noise);
      }
    };
    float compute(float x, float y);
    int sign(float x);

};

};
#endif
