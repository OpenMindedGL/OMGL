#include "ProcMixer.h"
#include <glm/glm.hpp>

OMGLProcMixer::OMGLProcMixer(std::vector<YGen*> * noises, float noisesSize ) : 
  ProcMixer(noises),
  NoiseGen(),
  m_NoisesSize(noisesSize)
{
  FastNoise* tp;
  for(unsigned int l = 1; glm::pow(2,l) <= m_Noises->size(); l++){
    tp = new FastNoise(m_Seed+l+(m_Octaves*4));
    tp->SetNoiseType(FastNoise::SimplexFractal);
    tp->SetFractalOctaves(8);
    tp->SetFrequency(m_Lacunarity * 0.01f * (1.0f/m_NoisesSize));
    m_Bits->push_back(tp);
  }
};

std::tuple<float, unsigned int> OMGLProcMixer::mix(float x, float y){
  float noise;
  float smooth_coef = 1.0f;
  float ret_value = 0.0f;
  unsigned int biome_choice;
  unsigned int d = 0;

  for(std::vector<FastNoise*>::iterator iter = m_Bits->begin(); iter < m_Bits->end(); ++iter){
    noise = (*iter)->GetNoise(x*10,y*10);
    biome_choice += sign(noise) * glm::pow(2,d);
    smooth_coef *= noise;
    d++;
  }
  biome_choice %= m_Noises->size();
  ret_value = m_Noises->at(biome_choice)->compute(x,y);
  smooth_coef = glm::fract(glm::abs(smooth_coef));

  if (smooth_coef > 0.5f)
    smooth_coef = 1.0f - smooth_coef;
  smooth_coef *= 2.0f;
  smooth_coef = glm::pow(smooth_coef, 0.25f);

  if ( glm::abs(m_Bits->at(0)->GetNoise(x*10,y*10)) < 0.04f){
    ret_value = -0.45f;
    smooth_coef = 1.0f;
  }

  return std::make_tuple(ret_value * smooth_coef,biome_choice);
}

int OMGLProcMixer::sign(float x){
  return x > 0.0f;
}
