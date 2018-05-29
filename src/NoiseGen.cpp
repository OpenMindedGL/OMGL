#include <stdio.h>
#include "NoiseGen.h"

NoiseGen::NoiseGen(){
  zoom = 50.0f;
  nbOctave = 5;
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
  /*simplex.SetNoiseType(FastNoise::Cubic); // Set the
                                       //desired noise type
  for(unsigned int k=0;k < nbOctave;k++){
    FastNoise temp_noise(seed);
    temp_noise.SetNoiseType(FastNoise::Simplex);
    temp_noise.SetFrequency(pow(lacunarity, k) * 0.01f * (1.0f/zoom) );
    simplex_fractal.push_back(temp_noise);
  }*/
  unsigned char perlinPermTable[256]={151,160,137,91,90,15, 
	131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
	190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
	88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,134,139,48,27,166,
	77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
	102,143,54,65,25,63,161,255,216,80,73,209,76,132,187,208,89,18,169,200,196,
	135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,250,124,123,
	5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
	223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,172,9,
	129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,228,
	251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107,
	49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180};  

  unsigned char perlinGradTable[16] = { 
	245, 176,
	176, 245,
	79, 245,
	10, 176,
	10, 79,
	79, 10,
	176, 10,
	245, 79};

  createPerlinPerm2DTable(perlinPermTable);
  createPerlinGrad2DTable(perlinPermTable, perlinGradTable);
}

void NoiseGen::createPerlinGrad2DTable(unsigned char (&perlinPermTable)[256], unsigned char (&perlinGradTable)[16])
{
  long int perlinGrad2DTable[256*256];
  long int * ptr = perlinGrad2DTable;

  // create optimized noise gradient-permutation table 
  // r = grad(perm(x) & 7).x
  // g = grad(perm(x) & 7).y
  // b = grad(perm(y) & 7).x
  // a = grad(perm(y) & 7).y

  for (int y = 0; y < 256; ++y){
    for (int x = 0; x < 256; ++x){
      unsigned char px = perlinPermTable[x];
      unsigned char py = perlinPermTable[y];
      *ptr++ = (perlinGradTable[((px & 7) << 1)] << 24) +
               (perlinGradTable[((px & 7) << 1) + 1] << 16) +
               (perlinGradTable[((py & 7) << 1)] << 8) +
               (perlinGradTable[((py & 7) << 1) + 1]);
    }
  }
  m_PerlinGrad2DTable = perlinGrad2DTable;
}

void NoiseGen::createPerlinPerm2DTable(unsigned char (&perlinPermTable)[256])
{
  long int perlinPerm2DTable[256 * 256];
  long int * ptr = perlinPerm2DTable;

  // create optimized noise permutation table 
  // r = perm(perm(x    ) + (y))
  // g = perm(perm(x    ) + (y + 1))
  // b = perm(perm(x + 1) + (y))
  // a = perm(perm(x + 1) + (y + 1))

  for (int y = 0; y < 256; ++y){
    for (int x = 0; x < 256; ++x){
      unsigned char a = perlinPermTable[x] + (unsigned char)y;
      unsigned char b = perlinPermTable[(x + 1) & 255] + (unsigned char)y;
      *ptr ++= (perlinPermTable[a] << 24) +
               (perlinPermTable[(a + 1) & 255] << 16) +
               (perlinPermTable[b] << 8) +
               (perlinPermTable[(b + 1) & 255]);
    }
  }
  m_PerlinPerm2DTable = perlinPerm2DTable;
}


glm::vec3 NoiseGen::noised(glm::vec2 p, float seed2){
  glm::vec2 i = glm::floor(p);
  glm::vec2 f = p - i;

  glm::vec2 w = f * f * f * (f * (f * glm::vec2(6.0f, 6.0f) - glm::vec2(15.0f, 15.0f)) + glm::vec2(10.0f, 10.0f));
  glm::vec4 w4(1, w.x, w.y, w.x * w.y);

  glm::vec2 dw = f * f * (f * (glm::vec2(30.0f, 30.0f) * f - glm::vec2(60.0f, 60.0f)) + glm::vec2(30.0f, 30.0f));

  glm::vec4 perm (m_PerlinPerm2DTable[(65536 + (((int)(i.x * 256 + i.y)) % 65536)) % 65536] + seed2,
  m_PerlinPerm2DTable[(65536 + (((int)(i.x * 256 + i.y + 1)) % 65536)) % 65536] + seed2,
  m_PerlinPerm2DTable[(65536 + (((int)((i.x + 1) * 256 + i.y)) % 65536)) % 65536] + seed2,
  m_PerlinPerm2DTable[(65536 + (((int)((i.x + 1) * 256 + i.y + 1)) % 65536)) % 65536] + seed2);

  glm::vec4 g1 (m_PerlinGrad2DTable[(65536 + (((int)(perm.x * 256 + perm.y)) % 65536)) % 65536] * 2 - 1,
  m_PerlinGrad2DTable[(65536 + (((int)(perm.x * 256 + perm.y + 1)) % 65536)) % 65536] * 2 - 1,
  m_PerlinGrad2DTable[(65536 + (((int)((perm.x + 1) * 256 + perm.y)) % 65536)) % 65536] * 2 - 1,
  m_PerlinGrad2DTable[(65536 + (((int)((perm.x + 1) * 256 + perm.y + 1)) % 65536)) % 65536] * 2 - 1);

  glm::vec4 g2 (m_PerlinGrad2DTable[(65536 + (((int)(perm.z * 256 + perm.w)) % 65536)) % 65536] * 2 - 1,
  m_PerlinGrad2DTable[(65536 + (((int)(perm.z * 256 + perm.w + 1)) % 65536)) % 65536] * 2 - 1,
  m_PerlinGrad2DTable[(65536 + (((int)((perm.z + 1) * 256 + perm.w)) % 65536)) % 65536] * 2 - 1,
  m_PerlinGrad2DTable[(65536 + (((int)((perm.z + 1) * 256 + perm.w + 1)) % 65536)) % 65536] * 2 - 1);

  glm::vec2 g1_xy(g1.x, g1.y);
  glm::vec2 g2_xy(g2.x, g2.y);
  glm::vec2 g1_zw(g1.z, g1.w);
  glm::vec2 g2_zw(g2.z, g2.w);
  float a = glm::dot(g1_xy, f);
  float b = glm::dot(g2_xy, f + glm::vec2(-1, 0));
  float c = glm::dot(g1_zw, f + glm::vec2(0, -1));
  float d = glm::dot(g2_zw, f + glm::vec2(-1, -1));

  glm::vec4 grads(a, b - a, c - a, a - b - c + d);
  float n = glm::dot(grads, w4);

  float dx = dw.x * (grads.y + (grads.w * w.y));
  float dy = dw.y * (grads.z + (grads.w * w.x));
  
  printf("n = %f\n", n);
  //printf("dx = %f, dy = %f", dx, dy);

  return glm::vec3(n * 1.5f, dx * 1.5f, dy * 1.5f);
}

float NoiseGen::compute(float x, float y){
  float sum = 0.5f;
  float freq = 1.0f;
  float amp = 1.0f;
  glm::vec2 dsum(0.0f, 0.0f);
  glm::vec2 p(x, y);
  for (int i = 0; i < nbOctave; i++){
    glm::vec3 n = noised(p*freq, (seed + i) / 256.0f);
    glm::vec2 n2(n.y, n.z);
    dsum += n2;
    sum += ((amp * n.x)/ (1 + glm::dot(dsum, dsum)));
    freq *= lacunarity;
    amp *= persistence;
  }
  return sum;

/*  float a,b,c;
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
}
