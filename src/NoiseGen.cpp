#include "NoiseGen.h"

NoiseGen::NoiseGen(){
  zoom = 50.0f;
  nbOctave = 6;
  lacunarity = 2.0f;
  persistence = 0.5f;
  seed = 69;
  mix_freq = 0.03f;
  simplex_freq = 0.03f;

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
    a += (*iter_noise).GetNoise(x*10,y*10) 
         * pow(persistence, d); 
    d++;
  }
  return a*zoom;
  /*b = simplex.GetNoise(x*10,y*10)*0.2f;
  c = ((mix.GetNoise(x*10,y*10)+1)/2.0f);
  return (a*(3*c*c) + b * (1-c) )*zoom;*/
  /*FastNoise p(0);
  p.SetNoiseType(FastNoise::SimplexFractal);
  p.SetFractalLacunarity(2.0f);
  p.SetFractalGain(0.5f);
  p.SetFractalOctaves(5);
  return p.GetNoise(x*10,y*10)*5.0f;*/

}
