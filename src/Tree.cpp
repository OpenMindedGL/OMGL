#include "Tree.h"
#include "Model.h"

#define DIVISIONS 10
#define SHRINK_RATE 0.8f
#define INIT_LENGTH 1.3f
#define INIT_RADIUS 0.1f
#define NOISYNESS   0.1f
#define FACES 10

Tree::Tree(){
  noise.SetNoiseType(FastNoise::Simplex);
  noise.SetFrequency(1000.0f);
  noise.SetSeed(14);
  generate(14);
  compute_indices();
  //Model<Vertexun>::ComputeNormals();
  Init(GL_TRIANGLE_STRIP, "shaders/Terrain.shader");
}


void Tree::generate(long int seed){
  glm::vec3 orig(0.0f,0.0f,0.0f);
  glm::vec3 dir(0.0f,1.0f,0.0f);
  float radius = INIT_RADIUS;
  float length = INIT_LENGTH;
  for(unsigned int i=1;i<DIVISIONS+1;i++){
    generateBranch(length, radius, orig, dir);
    radius*=SHRINK_RATE;
    length*=SHRINK_RATE;
    dir+=NOISYNESS*glm::vec3(noise.GetNoise(i,0), noise.GetNoise(i,1), noise.GetNoise(i,2));
    orig += dir * length;
  }
}

void Tree::generateBranch(float length, float radius, glm::vec3 origin, glm::vec3 direction){
  float a,b;
  unsigned int ind = 0;
  if(length < 0.1f)
    return;
  // make a circle
  for(unsigned int j=0;j<FACES;j++){
    // make a point
    a = cos((float)j*M_PI*2/FACES)*radius;
    b = sin((float)j*M_PI*2/FACES)*radius;
    m_Vertices.push_back(Vertexun(
          origin + glm::vec3(a,0,b),
          glm::vec2(),
          glm::vec3()
          ));
  }

}

void Tree::compute_indices(){
  
  unsigned int voffset = 0;
  for(unsigned int i=0;i<DIVISIONS-1;i++){
    for(unsigned int j=0;j<FACES;j++){
      m_Indices.push_back(j+voffset);
      m_Indices.push_back(j+voffset+FACES);
    }
    m_Indices.push_back(voffset);
    m_Indices.push_back(voffset+FACES);
    voffset += FACES;
    m_Indices.push_back(m_Vertices.size());     // restart index
  }
  
}
