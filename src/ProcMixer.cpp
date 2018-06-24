#include "ProcMixer.h"

float NoiseMixBiome::compute(float x, float y){
  float noise;
  float smooth_coef = 1.0f;
  float ret_value = 0.0f;
  unsigned int biome_choice;
  unsigned int d = 0;

  for(std::vector<FastNoise>::iterator iter = m_Biomes.begin(); iter < m_Biomes.end(); ++iter){
    noise = (*iter).GetNoise(x*10,y*10);
    biome_choice += sign(noise) * glm::pow(2,d);
    smooth_coef *= noise;
    d++;
  }
  ret_value = (*(myBiomes[biome_choice%(myBiomes.size())].m_Height)).compute(x,y);
  smooth_coef = glm::fract(glm::abs(smooth_coef));
  
  if (smooth_coef > 0.5f)
    smooth_coef = 1.0f - smooth_coef;
  smooth_coef *= 2.0f;
  smooth_coef = glm::pow(smooth_coef, 0.25f);

  if (glm::abs((*(m_Biomes.begin())).GetNoise(x*10,y*10)) < 0.04f){
    ret_value = -0.45f;
    smooth_coef = 1.0f;
  }

  return ret_value * smooth_coef;
}
