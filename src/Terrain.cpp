#include "Terrain.h"
#include "Model.h"
#include <GL/glew.h>

Terrain::Terrain(){
  initload(glm::vec2(0, 0)); 
  Model<Vertexun>(GL_TRIANGLE_STRIP, (std::vector<Vertexun>&) chunks, indices, shaderpath);
}



void Terrain::compute_indices(){
  for(unsigned int k=0;k<LENGTH*LENGTH;k++){
    for(unsigned int i=0;i<CHUNK_SIZE-1;i++){
      for(unsigned int j=0;j<CHUNK_SIZE;j++){
        indices.push_back((CHUNK_SIZE*CHUNK_SIZE*k)+j+CHUNK_SIZE*i);
        indices.push_back((CHUNK_SIZE*CHUNK_SIZE*k)+j+CHUNK_SIZE*(i+1));
      }
      indices.push_back(CHUNK_SIZE*CHUNK_SIZE*LENGTH);
    }
  }
}

void initload(glm::vec2 center){

  FastNoise* myNoise = (FastNoise*) malloc(nbOctave*sizeof(FastNoise));
  FastNoise valueNoise;
  FastNoise perlin(seed);
  perlin.SetNoiseType(FastNoise::Perlin); // Set the desired noise type
  perlin.SetFrequency( modfreq);
  valueNoise.SetNoiseType(FastNoise::Cubic); // Set the desired noise type
  for(unsigned int k=0;k<nbOctave;k++){
    myNoise[k] = FastNoise(seed);
    myNoise[k].SetNoiseType(FastNoise::Simplex); // Set the desired noise type
    myNoise[k].SetFrequency(pow(lacunarity, k) * 0.01f * (1.0f/zoom));
  }

  float a,b,c;
  for(unsigned int i=0;i<w;i++){
    for(unsigned int j=0;j<w;j++){
      a = 0;
      for(unsigned int k=0;k<nbOctave;k++){
        myNoise[k].SetFrequency(pow(lacunarity, k) * 0.01f * (1.0f/zoom));
        a += myNoise[k].GetNoise(i,j) * pow(persistence, k); 
      }
      b = valueNoise.GetNoise(i,j)*0.2f ;
      c = (perlin.GetNoise(i,j)+1)/2.0f;
      if (choosePerlin == 0)
        height_map[i][j] = a*(3*c*c) + b*(1-c);
      else if (choosePerlin == 1)
        height_map[i][j] = a;
      else if (choosePerlin == 2)
        height_map[i][j] = b;
      else
        height_map[i][j] = 3*c*c;
      height_map[i][j] *= zoom;
    }
  }
  free(myNoise);
}

void load(glm::vec2 chunkcoords){

}


