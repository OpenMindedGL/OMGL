#include "NoiseGen.h"

NoiseGen::NoiseGen(){
  zoom = 0;
  simplex = FastNoise(seed);
  mix = FastNoise(seed);
  mix.SetNoiseType(FastNoise::Perlin); // Set the desired
                                       // noise type
  mix.SetFrequency(mix_freq);
  simplex.SetNoiseType(FastNoise::Cubic); // Set the
                                       //desired noise type
  for(unsigned int k=0;k < nbOctave;k++){
    FastNoise temp_noise(seed);
    temp_noise.SetNoiseType(FastNoise::Simplex);
    temp_noise.SetFrequency(pow(lacunarity, k) * 0.01f * (1.0f/zoom) );
    simplex_fractal.push_back(temp_noise);
  }
}

float NoiseGen::compute(float x, float y){
  float a,b,c;
  a = 0;
  int d = 0;
  for(std::vector<FastNoise>::iterator iter_noise = 
  simplex_fractal.begin(); 
  iter_noise < simplex_fractal.end(); iter_noise++){
    a += (*iter_noise).GetNoise(x,y) 
         * pow(persistence, d); 
    d++;
  }
  b = simplex.GetNoise(x,y)*0.2f;
  c = (mix.GetNoise(x,y)+1)/2.0f;
  return a*(3*c*c) + b * (1-c);
}
