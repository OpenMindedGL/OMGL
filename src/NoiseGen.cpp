#include "NoiseGen.h"

NoiseGen::NoiseGen(){
  zoom = 50.0f;
  nbOctave = 8;
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
  float a,b,c/*,e*/;
  a = 0;
  int d = 0;
  for(std::vector<FastNoise>::iterator iter_noise = 
  simplex_fractal.begin(); 
  iter_noise < simplex_fractal.end(); iter_noise++){
    b = (*iter_noise).GetNoise(x*10,y*10) 
         * pow(persistence, d); 
    c = glm::abs(b);
    //e = 1 - c;
    a += lerp(glm::vec3(x,b,y),glm::vec3(x,c,y),0.7f).y;
    d++;
 /*or a+=(lerp(glm::vec3(x,b,y), glm::vec3(x,c,y), 0.7f).y + lerp(glm::vec3(x,b,y), glm::vec3(x,e,y), -0.7f).y) /2.0f;*/ /*or a+=lerp(glm::vec3(x,b,y), glm::vec3(x,e,y), -0.7f).y;*/
    /*a += glm::abs((*iter_noise).GetNoise(x*10,y*10) 
         * pow(persistence, d)); 
    d++;*/
  }
  return /*(-1)**/a*zoom;
  /*b = simplex.GetNoise(x*10,y*10)*0.2f;
  c = ((mix.GetNoise(x*10,y*10)+1)/2.0f);
  return (a*(3*c*c) + b * (1-c) )*zoom;*/
}

/*Vector3 Lerp(Vector3 start, Vector3 end, float percent)
{
     return (start + percent*(end - start));
}*/

glm::vec3 NoiseGen::lerp(glm::vec3 start, glm::vec3 end, float percent){
  return (start + percent*(end-start));
}
