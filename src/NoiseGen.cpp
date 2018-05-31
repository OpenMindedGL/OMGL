#include <stdio.h>
#include "NoiseGen.h"

NoiseGen::NoiseGen(){
  zoom = 3.0f;
  biomes_size_coef = 50.0f;
  nbOctave = 6;
  lacunarity = 2.00f;
  persistence = 0.5f;
  seed = 69;
  m_NbBiomes = 5;

  //m_Biomes.SetNoiseType(FastNoise::Cellular);
  //m_Biomes.SetFrequency(pow(lacunarity, 1) * 0.01f * (1.0f/biomes_size_coef));
  for(int l=0; l < m_NbBiomes - 1; l++){
    for(unsigned int k=0;k < 8;k++){
      FastNoise temp_noise(seed + k + l + (4*nbOctave));
      temp_noise.SetNoiseType(FastNoise::Simplex);
      temp_noise.SetFrequency(pow(lacunarity, k) * 0.01f * (1.0f/biomes_size_coef));
      m_Biomes.push_back(temp_noise);
    }
  }

  for(unsigned int k=0;k < nbOctave;k++){
    FastNoise temp_noise(seed+k);
    FastNoise temp_noise2(seed+k+nbOctave);
    FastNoise temp_noise3(seed+k+(2* nbOctave));
    temp_noise.SetNoiseType(FastNoise::Simplex);
    temp_noise2.SetNoiseType(FastNoise::Simplex);
    temp_noise3.SetNoiseType(FastNoise::Simplex);
    temp_noise.SetFrequency(pow(lacunarity, k) * 0.01f * (1.0f/zoom) );
    temp_noise2.SetFrequency(pow(lacunarity, k) * 0.01f * (1.0f/zoom) );
    temp_noise3.SetFrequency(pow(lacunarity, k) * 0.01f * (1.0f/zoom) );
    simplex_fractal.push_back(temp_noise);
    simplex_fractal2.push_back(temp_noise2);
    simplex_fractal3.push_back(temp_noise3);
  }
}

float NoiseGen::compute(float x, float y){
  int biome_choice = 0;
  float ret_value = 0.15f*10.0f;
  float temp;
  float m = 0;
  int d = 0;
  for (int i = 0; i < m_NbBiomes - 1; i++){
    for(std::vector<FastNoise>::iterator iter_noise = m_Biomes.begin() + i; 
    (iter_noise < m_Biomes.end() + i + 5); iter_noise++){
      m += (*iter_noise).GetNoise(x*10,y*10) * pow(persistence, d); 
      d++;
    }
    if (m > 0)
      biome_choice += 1;
    if ((m > -0.15f) && (m <= 0.0f)){
      temp = m*(-1)*10.0f;
      if (ret_value > temp)
        ret_value = temp;
    }
    else if ((m < 0.15f) && (m >= 0.0f)){
      temp = m*10.0f;
      if (ret_value > temp)
        ret_value = temp;
    }
    m = 0;
    d = 0;
  }

  if (ret_value <= 0.12f){
    ret_value = 0.12f;
  }
  else{
    switch (biome_choice){
      case 0:
        ret_value += compute_biome1(x, y);
        break;
      case 1:
        ret_value += compute_biome2(x, y);
        break;
      case 2:
        ret_value += compute_biome3(x, y);
        break;
      case 3:
        ret_value += compute_biome4(x, y);
        break;
      case 4:
        ret_value += compute_biome5(x, y);
        break;
      case 5:
        ret_value += compute_biome6(x, y);
        break;
      case 6:
        ret_value += compute_biome7(x, y);
        break;
      default:
        ret_value = m;
        break;
    }
  }
  return ret_value;
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
  float b = 0;
  float c = 0;
  int d = 0;
  float temp_lacunarity = 4.915f;
  float temp_persistence = 0.187f;
  std::vector<FastNoise>::iterator iter_noise2 = simplex_fractal2.begin();
  std::vector<FastNoise>::iterator iter_noise3 = simplex_fractal3.begin();
  for(std::vector<FastNoise>::iterator iter_noise = 
  simplex_fractal.begin(); 
  (iter_noise < simplex_fractal.end()); iter_noise++){
    (*iter_noise).SetFrequency(pow(temp_lacunarity, d) * 0.006f * (1.0f/zoom) );
    (*(iter_noise2 + d)).SetFrequency(pow(temp_lacunarity, d) * 0.006f * (1.0f/zoom) );
    (*(iter_noise3 + d)).SetFrequency(pow(temp_lacunarity, d) * 0.006f * (1.0f/zoom) );
    a += glm::abs((*iter_noise).GetNoise(x*10,y*10) * pow(temp_persistence, d)); 
    b += glm::abs((*(iter_noise2 + d)).GetNoise(x*10,y*10) * pow(temp_persistence, d)); 
    c += glm::abs((*(iter_noise3 + d)).GetNoise(x*10,y*10) * pow(temp_persistence, d)); 
    (*iter_noise).SetFrequency(pow(lacunarity, d) * 0.01f * (1.0f/zoom) );
    (*(iter_noise2 + d)).SetFrequency(pow(lacunarity, d) * 0.01f * (1.0f/zoom) );
    (*(iter_noise3 + d)).SetFrequency(pow(lacunarity, d) * 0.01f * (1.0f/zoom) );
    d++;
  }
  return a*b*c*3.0f*zoom;
}

float NoiseGen::compute_biome5(float x, float y){
  return compute_biome4(x,y)*(-1);
}

float NoiseGen::compute_biome6(float x, float y){
  FastNoise my_noise(seed + nbOctave*3);
  my_noise.SetNoiseType(FastNoise::Simplex);
  my_noise.SetFrequency(0.8f/zoom);
  float a = 0;
  float temp_lacunarity;
  int d = 0;
  for(std::vector<FastNoise>::iterator iter_noise = 
  simplex_fractal.begin(); 
  (iter_noise < simplex_fractal.end()); iter_noise++){
    temp_lacunarity = my_noise.GetNoise(x,y) + 0.5f;
    (*iter_noise).SetFrequency(pow(temp_lacunarity, d) * 0.01f * (1.0f/(zoom/5.0f))/5.0f);
    a += (*iter_noise).GetNoise(x*10,y*10) * pow(persistence, d); 
    (*iter_noise).SetFrequency(pow(lacunarity, d) * 0.01f * (1.0f/zoom) );
    d++;
  }
  return a*a*a*zoom/5.0f;
}

float NoiseGen::compute_biome7(float x, float y){
  FastNoise my_noise(seed + nbOctave*3);
  my_noise.SetNoiseType(FastNoise::Simplex);
  FastNoise my_noise2(seed + nbOctave*3);
  my_noise2.SetNoiseType(FastNoise::Simplex);
  my_noise.SetFrequency(my_noise2.GetNoise(x,y)/zoom);
  float a = 0;
  float temp_lacunarity;
  int d = 0;
  for(std::vector<FastNoise>::iterator iter_noise = 
  simplex_fractal.begin(); 
  (iter_noise < simplex_fractal.end()); iter_noise++){
    temp_lacunarity = my_noise.GetNoise(x,y) + 0.5f;
    (*iter_noise).SetFrequency(pow(temp_lacunarity, d) * 0.01f * (1.0f/(zoom/5.0f))/5.0f);
    a += (*iter_noise).GetNoise(x*10,y*10) * pow(persistence, d); 
    (*iter_noise).SetFrequency(pow(lacunarity, d) * 0.01f * (1.0f/zoom) );
    d++;
  }
  return a*a*a*zoom/5.0f;
}
