#include "Tree.h"
#include "Model.h"

#define DIVISIONS 10
#define SHRINK_RATE 2
#define INIT_LENGTH 2
#define INIT_RADIUS 0.5f
#define FACES 10

Tree::Tree(){
  generate(12);
  Model<Vertexun>::ComputeNormals();
  Init(GL_TRIANGLE_STRIP, "shaders/Terrain.shader");
}


void Tree::generate(long int seed){
  glm::vec3 orig(0.0f,0.0f,0.0f);
  glm::vec3 dir(0.2f,1.0f,0.3f);
  float radius = INIT_RADIUS;
  float length = INIT_LENGTH;
  m_Indices.resize((1+FACES)*2*50);
  unsigned int ind = 0;
  for(unsigned int i=1;i<10;i++){
    generateBranch(length, radius, orig, dir, i);
    radius*=0.7;
    length*=0.6;
    orig += dir * length;
  }
}

void Tree::generateBranch(float length, float radius, glm::vec3 origin, glm::vec3 direction, const unsigned int& ibranch){
  float a,b;
  unsigned int ind = 0;
  if(length < 0.1f)
    return;
  for(unsigned int i=0;i<2;i++){
    // make a circle
    ind = i+ibranch//*(FACES+1)*2;
    for(unsigned int j=0;j<FACES;j++){
      // make a point
      a = cos((float)j*M_PI*2/FACES)*radius;
      b = sin((float)j*M_PI*2/FACES)*radius;
      m_Vertices.push_back(Vertexun(
          origin + glm::vec3(a,0,b),
          glm::vec2(),
          glm::vec3()
          ));
      m_Indices[ind++] = j+FACES*i+ibranch*FACES*2;
      ind++;
    }
    // close the circle
    m_Indices[ind] = FACES*i;//+ibranch*FACES*2;
    origin += direction*length;
    radius*=0.7;
  }

}
