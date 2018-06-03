#include <stdio.h>
#include "NoiseGen.h"

NoiseGen::NoiseGen(){
  zoom = 30.0f;
  biomes_size_coef = 30.0f;
  nbOctave = 8;
  lacunarity = 2.00f;
  persistence = 0.5f;
  seed = 17;
  m_NbBiomes = 7;

//  FastNoise my_noise(54);
//  my_noise.SetFrequency(pow(lacunarity, 1) * 0.01f * (1.0f/10.0f));

  for(unsigned int l = 0; l < 3; l ++){
    FastNoise temp_noise(seed+l+(nbOctave*4));
    temp_noise.SetNoiseType(FastNoise::SimplexFractal);
    temp_noise.SetFractalOctaves(8);
    temp_noise.SetFrequency(lacunarity * 0.01f * (1.0f/biomes_size_coef));
    m_Biomes.push_back(temp_noise);
  }

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

int NoiseGen::sign(float x){
  if (x > 0.0f)
    return 1;
  else
    return 0;
}

float NoiseGen::compute(float x, float y){
  float smooth_coef;
  float ret_value = 0.0f;
  std::vector<FastNoise>::iterator iter1 = m_Biomes.begin();
  std::vector<FastNoise>::iterator iter2 = m_Biomes.begin() + 1;
  std::vector<FastNoise>::iterator iter3 = m_Biomes.end();
  float noise1 = (*iter1).GetNoise(x*10,y*10);
  float noise2 = (*iter2).GetNoise(x*10,y*10);
  float noise3 = (*iter3).GetNoise(x*10,y*10);
  int biome_choice = sign(noise1) * 4 + sign(noise2) * 2 + sign(noise3);

  switch (biome_choice){
    case 0:
      ret_value = compute_biome1(x, y);
      break;
    case 1:
      ret_value = compute_biome2(x, y);
      break;
    case 2:
    case 7:
      ret_value = compute_biome3(x, y);
      break;
    case 3:
      ret_value = compute_biome4(x, y);
      break;
    case 4:
      ret_value = compute_biome5(x, y);
      break;
    case 5:
      ret_value = compute_biome6(x, y);
      break;
    case 6:
      ret_value = compute_biome7(x, y);
      break;
    default:
      break;
  }
  smooth_coef = glm::fract(glm::abs(noise1 * noise2 * noise3));
  if (smooth_coef > 0.5f)
    smooth_coef = 1.0f - smooth_coef;
  smooth_coef *= 2.0f;
  smooth_coef = glm::pow(smooth_coef, 0.5f);
  return ret_value * smooth_coef;
}

float NoiseGen::compute_biome1(float x, float y){
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

int NoiseGen::compute_biome2(float x, float y){
  return (glm::floor(compute_biome1(x,y)));
}

float NoiseGen::compute_biome3(float x, float y){
  float ret_value;
  float temp_persistence = persistence;
  persistence = 0.154f;
  ret_value = glm::abs(compute_biome1(x,y)) * (-1);
  persistence = temp_persistence;
  return ret_value;
}

float NoiseGen::compute_biome4(float x, float y){
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

float NoiseGen::compute_biome5(float x, float y){
  return compute_biome4(x,y)*(-1);
}

float NoiseGen::compute_biome6(float x, float y){
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

float NoiseGen::compute_biome7(float x, float y){
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
