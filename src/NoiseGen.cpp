#include "NoiseGen.h"
#include "Gui.h"

Gui gui;

NoiseGen::NoiseGen(){
  zoom = 50.0f;
  nbOctave = 2;
  lacunarity = 2.0f;
  persistence = 0.5f;
  seed = 69;
  mix_freq = 0.03f;
  simplex_freq = 0.03f;

  simplex = FastNoise(seed);
  /*mix = FastNoise(seed);
  mix.SetNoiseType(FastNoise::Perlin); // Set the desired
                                       // noise type
  mix.SetFrequency(mix_freq);*/
  simplex.SetNoiseType(FastNoise::Simplex); // Set the
  /*                                     //desired noise type
  for(unsigned int k=0;k < nbOctave;k++){
    FastNoise temp_noise(seed);
    temp_noise.SetNoiseType(FastNoise::Simplex);
    temp_noise.SetFrequency(pow(lacunarity, k) * 0.01f * (1.0f/zoom) );
    simplex_fractal.push_back(temp_noise);
  }*/
  
}

float NoiseGen::hash( glm::vec3 p){
  //return simplex.GetNoise(p.x*20.0f,p.y*20.0f,p.z*20.0f);
    //return glm::fract( n*17.0f*glm::fract( n*0.3183099f ) );
    p  = 50.0f*glm::fract( p*0.3183099f + glm::vec3(0.71f,0.113f,0.419f));
    return -1.0f+2.0f*glm::fract( p.x*p.y*p.z*(p.x+p.y+p.z) );
}

glm::vec4 NoiseGen::noised(glm::vec3 x){
    glm::vec3 p = glm::floor(x);
    glm::vec3 w = glm::fract(x);
    
    glm::vec3 u(w*w*w*(w*(w*6.0f-15.0f)+10.0f));
    glm::vec3 du(30.0f*w*w*(w*(w-2.0f)+1.0f));

    //float n = p.x + 317.0f*p.y + 157.0f*p.z;
    /*vec3 u = w*w*(3.0-2.0*w);
    vec3 du = 6.0*w*(1.0-w);*/
    
    float a = hash(p+glm::vec3(0.0,0.0,0.0));
    float b = hash(p+glm::vec3(1.0,0.0,0.0));
    float c = hash(p+glm::vec3(0.0,1.0,0.0));
    float d = hash(p+glm::vec3(1.0,1.0,0.0));
    float e = hash(p+glm::vec3(0.0,0.0,1.0));
    float f = hash(p+glm::vec3(1.0,0.0,1.0));
    float g = hash(p+glm::vec3(0.0,1.0,1.0));
    float h = hash(p+glm::vec3(1.0,1.0,1.0));
	
    float k0 =   a;
    float k1 =   b - a;
    float k2 =   c - a;
    float k3 =   e - a;
    float k4 =   a - b - c + d;
    float k5 =   a - c - e + g;
    float k6 =   a - b - e + f;
    float k7 = - a + b + c - d + e - f - g + h;

    return glm::vec4(
             k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z 
             + k6*u.z*u.x + k7*u.x*u.y*u.z, 
             du * glm::vec3( k1 + k4*u.y + k6*u.z + k7*u.y*u.z,
                        k2 + k5*u.z + k4*u.x + k7*u.z*u.x,
                        k3 + k6*u.x + k5*u.y + k7*u.x*u.y ) );
}

float NoiseGen::compute(float x, float y){
  //return hash(glm::vec3(x,0,y*0.0001));
  //return simplex.GetNoise(x*20.0f,y*20.0f);
  //glm::vec3 r(x, simplex.GetNoise(x*0.1f,y*0.1f),y);
  glm::vec3 r(x*5.0f, 0.0f,y*5.0f);
  glm::mat3 m3( 0.0f,  0.8f,  0.6f,
               -0.8f,  0.36f, -0.48f,
               -0.6f, -0.48f,  0.64f );
  glm::mat3 m3i( 0.0f, -0.8f, -0.6f,
                 0.8f,  0.36f, -0.48f,
                 0.6f, -0.48f,  0.64f );
  float f = 2.0f;
  float s = 0.5f;
  float a = 0.0f;
  float b = 0.5f;
  glm::vec3 d(0.0f, 0.0f, 0.0f);
  glm::mat3 m(1.0f,0.0f,0.0f,
              0.0f,1.0f,0.0f,
              0.0f,0.0f,1.0f);
  for(int i=0; i < nbOctave; i++ ){
    glm::vec4 n = (*this).noised(r);
    glm::vec3 n2(n.y, n.z, n.w); 
    a += b*n.x;          // accumulate values		
    d += b*m*n2;
    b *= s;
    r = f*m3*r;
    m = f*m3i*m;
  }
  return d.x*d.y*d.z*a;      // *(1.0f/3.0f) //vec4( a, d );

  /*float a,b,c;
  a = 0;
  int d = 0;
  for(std::vector<FastNoise>::iterator iter_noise = 
  simplex_fractal.begin(); 
  iter_noise < simplex_fractal.end(); iter_noise++){
    a += (*iter_noise).GetNoise(x*10,y*10) 
         * pow(persistence, d); 
    d++;
  }
  return a*zoom;*/
  /*b = simplex.GetNoise(x*10,y*10)*0.2f;
  c = ((mix.GetNoise(x*10,y*10)+1)/2.0f);
  return (a*(3*c*c) + b * (1-c) )*zoom;*/

}

