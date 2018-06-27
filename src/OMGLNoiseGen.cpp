#include <stdio.h>
#include <math.h>
#include <glm/glm.hpp>
#include "NoiseGen.h"
#include "FastNoise.h"
#include "OMGLNoiseGen.h"

OMGLNoiseGen::OMGLNoiseGen( unsigned int nbNoises ) :
  NoiseGen()
{
  m_Noises.resize(nbNoises);
  long long int sd;
  FastNoise* tp;
  for(unsigned int j=0;j < nbNoises;j++){
    sd = m_Seed + j * m_Octaves;
    for(unsigned int k=0;k < m_Octaves;k++){
      sd+=k;
      tp = new FastNoise(sd);
      tp->SetNoiseType(FastNoise::Simplex);
      tp->SetFrequency(pow(m_Lacunarity, k) * 0.01f / m_Zoom);
      m_Noises[j].push_back(tp);
    }
  }

}

float* OMGLNoiseCloud::compute2(float x, float y){
  float a = 0;
  float c = 0;
  float b = m_Mix->GetNoise(x*10,y*10);
  int d = 0;
  float e,f, ret_val;
  for(std::vector<FastNoise*>::iterator iter_noise = 
  m_Noises[0].begin(); 
  (iter_noise < m_Noises[0].end()); iter_noise++){
    a += glm::abs((*iter_noise)->GetNoise(x*10,y*10) * pow(m_Persistence, d)); 
    if ((d>0) && (d <= 3))
      c += glm::abs((*iter_noise)->GetNoise(x*10,y*10) * pow(m_Persistence, d)); 
    d++;
  }
 
  if ((b > 0.1f) && (a > 0.5f)){
    e = glm::abs(m_Mix->GetNoise(x*10,y*10) * pow(m_Persistence, 0)); 
    m_Mix->SetFractalOctaves(1);
    f = m_Mix->GetNoise(x*10,y*10);
    m_Mix->SetFractalOctaves(8);
    ret_val = e+f;
    ret_val = glm::pow(ret_val,3)*c + 50.0f +c;
    float* h = new float[4]{ret_val,a,b,c};
    return h;
  }
  else{
    float* g = new float[4]{1200,0,0,0};
    return g;
  }
}

float OMGLNoiseCloudRecto::compute(float x, float y){
  float* r=compute2(x,y); 
  if(r[0] != 1200.0f)
    r[0] -= (r[1]+r[2]-0.6f)*r[3]*4.0f;
  return r[0];
}



float OMGLNoiseBiome::compute1(float x, float y){
  float a = 0;
  float b = 0;
  float c = 0;
  int d = 0;
  std::vector<FastNoise*>::iterator iter_noise2 = m_Noises[1].begin();
  std::vector<FastNoise*>::iterator iter_noise3 = m_Noises[2].begin();
  for(std::vector<FastNoise*>::iterator iter_noise = 
  m_Noises[0].begin(); 
  (iter_noise < m_Noises[0].end()); iter_noise++){
    a += (*iter_noise)->GetNoise(x*10,y*10) * pow(m_Persistence, d); 
    b += (*(iter_noise2 + d))->GetNoise(x*10,y*10) * pow(m_Persistence, d); 
    c += (*(iter_noise3 + d))->GetNoise(x*10,y*10) * pow(m_Persistence, d); 
    d++;
  }
  return (a*b*c*m_Zoom)/3.0f;
}

float OMGLNoiseBiome::compute4(float x, float y){
  float a = 0;
  int d = 0;
  float temp_lacunarity = 4.915f;
  float temp_persistence = 0.187f;
  for(std::vector<FastNoise*>::iterator iter_noise = 
  m_Noises[0].begin(); 
  (iter_noise < m_Noises[0].end()); iter_noise++){
    (*iter_noise)->SetFrequency(pow(temp_lacunarity, d) * 0.006f / m_Zoom);
    a += glm::abs((*iter_noise)->GetNoise(x*10,y*10) * pow(temp_persistence, d)); 
    (*iter_noise)->SetFrequency(pow(m_Lacunarity, d) * 0.01f / m_Zoom);
    d++;
  }
  return a*a*a/**1.5f*m_Zoom*/;
}

float OMGLNoiseBiome1::compute(float x, float y){
  float a =compute1(x, y); 
  if(a < -1.0f || a > 1.0f)
    printf("THIS CAN NOT HAPPEND %f, zoom:%f\n", a, m_Zoom);
  return a;
}

float OMGLNoiseBiome2::compute(float x, float y){
  return (glm::floor(compute1(x, y)*100)/100);
}

float OMGLNoiseBiome3::compute(float x, float y){
  float ret_value;
  float temp_m_Persistence = m_Persistence;
  m_Persistence = 0.154f;
  ret_value = glm::abs(compute1(x,y)) * (-1);
  m_Persistence = temp_m_Persistence;
  return ret_value;
}


float OMGLNoiseBiome4::compute(float x, float y){
  return compute4(x, y);
}

float OMGLNoiseBiome5::compute(float x, float y){
  return compute4(x,y)*(-1);
}

float OMGLNoiseBiome6::compute(float x, float y){
  FastNoise my_noise(m_Seed + m_Octaves*4);
  my_noise.SetNoiseType(FastNoise::Simplex);
  my_noise.SetFrequency(0.8f/m_Zoom);
  float a = 0;
  float temp_lacunarity;
  int d = 0;
  for(std::vector<FastNoise*>::iterator iter_noise = 
  m_Noises[0].begin(); 
  (iter_noise < m_Noises[0].end()); iter_noise++){
    temp_lacunarity = my_noise.GetNoise(x,y) + 0.5f;
    (*iter_noise)->SetFrequency(pow(temp_lacunarity, d) * 0.01f / m_Zoom);
    a += (*iter_noise)->GetNoise(x*10,y*10) * pow(m_Persistence, d); 
    (*iter_noise)->SetFrequency(pow(m_Lacunarity, d) * 0.01f / m_Zoom);
    d++;
  }
  return a*a*a*m_Zoom/5.0f;
}

float OMGLNoiseBiome7::compute(float x, float y){
  FastNoise my_noise(m_Seed + m_Octaves*4 + 1);
  my_noise.SetNoiseType(FastNoise::Simplex);
  FastNoise my_noise2(m_Seed + m_Octaves*4 + 2);
  my_noise2.SetNoiseType(FastNoise::Simplex);
  my_noise.SetFrequency(my_noise2.GetNoise(x,y)/m_Zoom);
  float a = 0;
  float temp_lacunarity;
  int d = 0;
  for(std::vector<FastNoise*>::iterator iter_noise = 
  m_Noises[0].begin(); 
  (iter_noise < m_Noises[0].end()); iter_noise++){
    temp_lacunarity = my_noise.GetNoise(x,y) + 0.5f;
    (*iter_noise)->SetFrequency(pow(temp_lacunarity, d) * 0.01f / m_Zoom);
    a += (*iter_noise)->GetNoise(x*10,y*10) * pow(m_Persistence, d); 
    (*iter_noise)->SetFrequency(pow(m_Lacunarity, d) * 0.01f / m_Zoom);
    d++;
  }
  return a*a*a*m_Zoom/5.0f;
}
