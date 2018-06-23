#include <stdio.h>
#include "NoiseGen.h"

NoiseGen::NoiseGen(){
  zoom = 3.0f;
  biomes_size_coef = 15.0f;
  nbOctave = 8;
  lacunarity = 2.00f;
  persistence = 0.5f;
  seed = 62;

  for(unsigned int k=0;k < nbOctave;k++){
    FastNoise temp_noise(seed+k);
    FastNoise temp_noise2(seed+k+nbOctave);
    FastNoise temp_noise3(seed+k+(2* nbOctave));
    temp_noise.SetNoiseType(FastNoise::Simplex);
    temp_noise2.SetNoiseType(FastNoise::Simplex);
    temp_noise3.SetNoiseType(FastNoise::Simplex);
    temp_noise.SetFrequency(pow(lacunarity, k) * 0.01f / zoom);
    temp_noise2.SetFrequency(pow(lacunarity, k) * 0.01f / zoom);
    temp_noise3.SetFrequency(pow(lacunarity, k) * 0.01f / zoom);
    simplex_fractal.push_back(temp_noise);
    simplex_fractal2.push_back(temp_noise2);
    simplex_fractal3.push_back(temp_noise3);
  }
}

float NoiseCloudVerso::compute(float x, float y){
  float a = 0;
  float c = 0;
  float b = (*(m_Biomes.begin())).GetNoise(x*10,y*10);
  int d = 0;
  float e,f, ret_val;
  for(std::vector<FastNoise>::iterator iter_noise = 
  simplex_fractal.begin(); 
  (iter_noise < simplex_fractal.end()); iter_noise++){
    a += glm::abs((*iter_noise).GetNoise(x*10,y*10) * pow(persistence, d)); 
    if ((d>0) && (d <= 3))
      c += glm::abs((*iter_noise).GetNoise(x*10,y*10) * pow(persistence, d)); 
    d++;
  }
 
  if ((b > 0.1f) && (a > 0.5f)){
    e = glm::abs((*(simplex_fractal.begin())).GetNoise(x*10,y*10) * pow(persistence, 0)); 
    (*(m_Biomes.begin())).SetFractalOctaves(1);
    f = (*(m_Biomes.begin())).GetNoise(x*10,y*10);
    (*(m_Biomes.begin())).SetFractalOctaves(8);
    ret_val = e+f;
    ret_val = glm::pow(ret_val,3)*c + 50.0f +c;
    return ret_val;
  }
  else
    return 1200.0f;
}

float NoiseCloudRecto::compute(float x, float y){
  float a = 0;
  float c = 0;
  float b = (*(m_Biomes.begin())).GetNoise(x*10,y*10);
  int d = 0;
  float e,f, ret_val;
  for(std::vector<FastNoise>::iterator iter_noise = 
  simplex_fractal.begin(); 
  (iter_noise < simplex_fractal.end()); iter_noise++){
    a += glm::abs((*iter_noise).GetNoise(x*10,y*10) * pow(persistence, d)); 
    if ((d>0) && (d <= 3))
      c += glm::abs((*iter_noise).GetNoise(x*10,y*10) * pow(persistence, d)); 
    d++;
  }
 
  if ((b > 0.1f) && (a > 0.5f)){
    e = glm::abs((*(simplex_fractal.begin())).GetNoise(x*10,y*10) * pow(persistence, 0)); 
    (*(m_Biomes.begin())).SetFractalOctaves(1);
    f = (*(m_Biomes.begin())).GetNoise(x*10,y*10);
    (*(m_Biomes.begin())).SetFractalOctaves(8);
    ret_val = e+f;
    ret_val = glm::pow(ret_val,3)*c + 50.0f +c;
    ret_val = ret_val - (a+b-0.6f)*c*4.0f;
    return ret_val;
  }
  else
    return 1200.0f;
}

int NoiseMixBiome::sign(float x){
  if (x > 0.0f)
    return 1;
  else
    return 0;
}

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

float NoiseGen::compute1(float x, float y){
  float a = 0;
  float b = 0;
  float c = 0;
  int d = 0;
  std::vector<FastNoise>::iterator iter_noise2 = simplex_fractal2.begin();
  std::vector<FastNoise>::iterator iter_noise3 = simplex_fractal3.begin();
  for(std::vector<FastNoise>::iterator iter_noise = 
  simplex_fractal.begin(); 
  (iter_noise < simplex_fractal.end()); iter_noise++){
    a += (*iter_noise).GetNoise(x*10,y*10) * pow(persistence, d); 
    b += (*(iter_noise2 + d)).GetNoise(x*10,y*10) * pow(persistence, d); 
    c += (*(iter_noise3 + d)).GetNoise(x*10,y*10) * pow(persistence, d); 
    d++;
  }
  return a*b*c*zoom;
}

float NoiseBiome1::compute(float x, float y){
  return compute1(x, y);
}

float NoiseBiome2::compute(float x, float y){
  return (glm::floor(compute1(x, y)));
}

float NoiseBiome3::compute(float x, float y){
  float ret_value;
  float temp_persistence = persistence;
  persistence = 0.154f;
  ret_value = glm::abs(compute1(x,y)) * (-1);
  persistence = temp_persistence;
  return ret_value;
}

float NoiseGen::compute4(float x, float y){
  float a = 0;
  int d = 0;
  float temp_lacunarity = 4.915f;
  float temp_persistence = 0.187f;
  for(std::vector<FastNoise>::iterator iter_noise = 
  simplex_fractal.begin(); 
  (iter_noise < simplex_fractal.end()); iter_noise++){
    (*iter_noise).SetFrequency(pow(temp_lacunarity, d) * 0.006f / zoom);
    a += glm::abs((*iter_noise).GetNoise(x*10,y*10) * pow(temp_persistence, d)); 
    (*iter_noise).SetFrequency(pow(lacunarity, d) * 0.01f / zoom);
    d++;
  }
  return a*a*a*1.5f*zoom;
}

float NoiseBiome4::compute(float x, float y){
  return compute4(x, y);
}

float NoiseBiome5::compute(float x, float y){
  return compute4(x,y)*(-1);
}

float NoiseBiome6::compute(float x, float y){
  FastNoise my_noise(seed + nbOctave*4);
  my_noise.SetNoiseType(FastNoise::Simplex);
  my_noise.SetFrequency(0.8f/zoom);
  float a = 0;
  float temp_lacunarity;
  int d = 0;
  for(std::vector<FastNoise>::iterator iter_noise = 
  simplex_fractal.begin(); 
  (iter_noise < simplex_fractal.end()); iter_noise++){
    temp_lacunarity = my_noise.GetNoise(x,y) + 0.5f;
    (*iter_noise).SetFrequency(pow(temp_lacunarity, d) * 0.01f / zoom);
    a += (*iter_noise).GetNoise(x*10,y*10) * pow(persistence, d); 
    (*iter_noise).SetFrequency(pow(lacunarity, d) * 0.01f / zoom);
    d++;
  }
  return a*a*a*zoom/5.0f;
}

float NoiseBiome7::compute(float x, float y){
  FastNoise my_noise(seed + nbOctave*4 + 1);
  my_noise.SetNoiseType(FastNoise::Simplex);
  FastNoise my_noise2(seed + nbOctave*4 + 2);
  my_noise2.SetNoiseType(FastNoise::Simplex);
  my_noise.SetFrequency(my_noise2.GetNoise(x,y)/zoom);
  float a = 0;
  float temp_lacunarity;
  int d = 0;
  for(std::vector<FastNoise>::iterator iter_noise = 
  simplex_fractal.begin(); 
  (iter_noise < simplex_fractal.end()); iter_noise++){
    temp_lacunarity = my_noise.GetNoise(x,y) + 0.5f;
    (*iter_noise).SetFrequency(pow(temp_lacunarity, d) * 0.01f / zoom);
    a += (*iter_noise).GetNoise(x*10,y*10) * pow(persistence, d); 
    (*iter_noise).SetFrequency(pow(lacunarity, d) * 0.01f / zoom);
    d++;
  }
  return a*a*a*zoom/5.0f;
}
