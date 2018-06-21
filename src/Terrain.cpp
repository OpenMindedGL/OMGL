#include "Terrain.h"
#include "Mesh.h"
#include "Vertex.h"
#include "HeightMap.h"
#include "Debug.h"
#include "Log.h"
#include <vector>
#include "NoiseGen.h"
#include <glm/glm.hpp>



Terrain::Terrain(glm::vec2 spawn, float p, unsigned int s, unsigned int n) :
  m_Precision(p),
  m_NbLevels(n),
  m_Noise(),
  m_Center(static_cast<glm::i32vec2>(spawn)),
  m_Scale(64.0f)
{
  m_Size = s;

  if((m_Size-3) % 4 != 0){
    printf("[WARNING] trying to make a LODLevel with a size-3 not dividable by 4, bad things gonna happend, you've been warned\n");
  }
  m_Shader =new Shader("shaders/Terrain.shader");
  int size = s+2;

  LODLevel::GenMeshes(m_Size);   
  LODLevel* l;
  for(unsigned int i=0;i<m_NbLevels;i++){
    m_Lods[i] = new LODLevel(i,spawn,this);
  }

}


void Terrain::Update(glm::i32vec2& center){

  /* this is quite ugly stuff */
  glm::i32vec2 dir = center - m_Center;
  m_Center = center;
  /*glm::i32vec2 d = glm::i32vec2(0);//( dir / glm::i32vec2(2) ) * glm::i32vec2(2);
  int a = 1;
  if(dir.x > 0)
    a = 0;
  if(glm::abs(center.x) % 2 == a && dir.x != 0)
    d.x = dir.x;
  a = 1;
  if(dir.y > 0)
    a = 0;
  if(glm::abs(center.y) % 2 == a && dir.y != 0)
    d.y = dir.y;*/
  //m_HeightMap->Update(d);
  //m_Center+=d;
  /* -------------------- */

  for(unsigned int i=0;i<NB_LEVELS;i++){
    m_Lods[i]->Update(center);

    
      //lods[i]->UnmapBuffer();
//      lods[i]->Upload();
      //lods[i]->ComputeIndices();
      //lods[i]->UploadIndexBuffer();
      //Log::PrintVertices(*(lods[i]->GetVertices()));
      //Log::PrintIndices(*(lods[i]->GetIndices()));
      /*Vertexun* b =(Vertexun*) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
      GLCall(std::vector<Vertexun> a(b,b+(*(lods[i]->GetVertices())).size()) ); 
      Log::PrintVertices(a);
      //printf("%f \n",(*a).pos);
      GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));*/
    }
  }



