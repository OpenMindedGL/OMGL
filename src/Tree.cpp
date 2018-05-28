#include "Tree.h"
#include "Model.h"

#define DIVISIONS 7
#define SHRINK_RATE 0.7f
#define INIT_LENGTH 1.3f
#define INIT_RADIUS 0.1f
#define NOISYNESS   0.6f
#define FACES 10

Tree::Tree(){
  noise.SetNoiseType(FastNoise::Simplex);
  noise.SetFrequency(1.0f);
  noise.SetSeed(15);
  generate(14);
  //compute_indices();
  //Model<Vertexun>::ComputeNormals();
  Init(GL_TRIANGLE_STRIP, "shaders/Terrain.shader");
}


void Tree::generate(long int seed){
  glm::vec3 orig(0.0f,0.0f,0.0f);
  glm::vec3 dir(0.0f,1.0f,0.0f);
  float radius = INIT_RADIUS;
  float length = INIT_LENGTH;
  unsigned int ioffset = 0;
  generateBranch(length, radius, orig, dir,0,ioffset,0);
}

void Tree::generateBranch(float length, float radius, glm::vec3 orig, glm::vec3 dir, unsigned int vparent, unsigned int& voffset, unsigned int div){
  float a,b;
  unsigned int ind = 0;
  // make a circle
  for(unsigned int j=0;j<FACES;j++){
    // make a point
    a = cos((float)j*M_PI*2/FACES)*radius;
    b = sin((float)j*M_PI*2/FACES)*radius;
    m_Vertices.push_back(Vertexun(
          orig + glm::vec3(a,0,b),
          glm::vec2(),
          glm::vec3()
          ));
  }
  if(div != 0){
    // the root is an orphan
    for(unsigned int j=0;j<FACES;j++){
      m_Indices.push_back(j+vparent);
      m_Indices.push_back(j+voffset);
    }
    m_Indices.push_back(vparent);
    m_Indices.push_back(voffset);
    m_Indices.push_back(pow(2,DIVISIONS)*FACES);     // restart index
    
  
  }
  unsigned int parent = voffset;
  voffset+=FACES;
  if(div == DIVISIONS)
    return;
  radius*=SHRINK_RATE;
  length*=SHRINK_RATE;
  if(div != 0){
    glm::vec3 dir1 = dir + NOISYNESS*glm::vec3(noise.GetNoise(voffset,0), noise.GetNoise(voffset,1), noise.GetNoise(voffset,2));
    // glm::vec3 dir1 = dir + glm::vec3(0.4f,1.0f,0.0f);
    generateBranch(length, radius, orig+dir1*length, dir1,parent,voffset,div+1);
    dir += NOISYNESS*glm::vec3(noise.GetNoise(voffset,3), noise.GetNoise(voffset,4), noise.GetNoise(voffset,5));
    //dir += glm::vec3(0.0f,1.0f,0.4f);
    generateBranch(length, radius, orig+dir*length, dir,parent,voffset,div+1);
  }
  else{
    generateBranch(length, radius, orig+dir*length, dir,parent,voffset,div+1);
  }

}

void Tree::compute_indices(){
  
  unsigned int voffset = 0;
  for(unsigned int i=0;i<pow(2,DIVISIONS)-2;i++){
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
