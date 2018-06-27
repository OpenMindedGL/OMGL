#include "ProcMixer.h"
#include <glm/glm.hpp>

OMGLProcMixer::OMGLProcMixer(std::vector<YGen*> * noises, float noisesSize ) : 
  ProcMixer(noises),
  NoiseGen(),
  m_NoisesSize(noisesSize)
{
  if(m_Noises->size() > 1){
    // more than 1 noise to mix
    // make some noises to mix them
    FastNoise* tp;
    m_Bits = new std::vector<FastNoise*>(0);
    for(unsigned int l = 1; glm::pow(2,l) <= m_Noises->size(); l++){
      tp = new FastNoise(m_Seed+l+(m_Octaves*4));
      tp->SetNoiseType(FastNoise::SimplexFractal);
      tp->SetFractalOctaves(8);
      tp->SetFrequency(m_Lacunarity * 0.01f * (1.0f/m_NoisesSize));
      m_Bits->push_back(tp);
    }
  }
};

std::tuple<float, unsigned int> OMGLProcMixer::mix(float x, float y){
  if(m_Noises->size() > 1){
    // more than 1 noise to mix
    // choose one
    float noise;
    float smooth_coef = 1.0f;
    float ret_value = 0.0f;
    unsigned int biome_choice = 0;
    unsigned int d = 0;

    for(std::vector<FastNoise*>::iterator iter = m_Bits->begin(); iter < m_Bits->end(); ++iter){
      noise = (*iter)->GetNoise(x,y);
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

    /*if ( glm::abs(m_Bits->at(0)->GetNoise(x*10,y*10)) < 0.04f){
      ret_value = -0.45f;
      smooth_coef = 1.0f;
    }*/

    return std::make_tuple((ret_value * smooth_coef+1)/2.0f,biome_choice);
  }
  else
    // only one noise, just return it
    return std::make_tuple((m_Noises->at(0)->compute(x,y)+1)/2.0f,0);
}

int OMGLProcMixer::sign(float x){
  return x > 0.0f;
}
