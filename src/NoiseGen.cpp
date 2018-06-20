#include <stdio.h>
#include "NoiseGen.h"

NoiseGen::NoiseGen(){
  zoom = 10.0f;
  nbOctave = 8;
  lacunarity = 2.00f;
  persistence = 0.5f;
  seed = 69;

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
  float a = 0;
  float b = 0;
  float c = 0;
  int d = 0;
  //int e = nbOctave;
  float abs;
  std::vector<FastNoise>::iterator iter_noise2 = simplex_fractal2.begin();
  std::vector<FastNoise>::iterator iter_noise3 = simplex_fractal3.begin();
  for(std::vector<FastNoise>::iterator iter_noise = 
  simplex_fractal.begin(); 
  (iter_noise < simplex_fractal.end())/* && (d < e)*/; iter_noise++){
    a += (*iter_noise).GetNoise(x*10,y*10) * pow(persistence, d); 
    b += (*(iter_noise2 + d)).GetNoise(x*10,y*10) * pow(persistence, d); 
    c += (*(iter_noise3 + d)).GetNoise(x*10,y*10) * pow(persistence, d); 
    d++;
    /*abs = glm::abs(a*b*c);
    if (iter_noise == simplex_fractal.begin()){
      if (abs > 0.8f)
        e = nbOctave - 4;
      else if ((abs >= 0.6f) && (abs < 0.8f))
        e = nbOctave - 3;
      else if ((abs >= 0.4f) && (abs < 0.6f))
        e = nbOctave - 2;
      else if ((abs >= 0.2f) && (abs < 0.4f))
        e = nbOctave - 1;
    }*/
  }
  
  return a*b*c*zoom;  //WAS THIS ORIGINALY
  //return y/20;
  //return glm::sin(x/30)*10;
  //return a*b*c;
}
