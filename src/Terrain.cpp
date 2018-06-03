#include "Terrain.h"
#include "Model.h"
#include "Vertex.h"
#include <vector>
#include "NoiseGen.h"
#include <glm/glm.hpp>


Terrain::Terrain(float p, unsigned int s, unsigned int n, glm::vec2 spawn) :
  m_Precision(p),
  m_Size(s),
  m_HalfSize(s/2),
  m_NbLevels(n)
{
  
  LODLevel* l;
  Material* m = new Material();
  for(unsigned int i=0;i<NB_LEVELS;i++){
    l = new LODLevel(i,spawn,this);
    lods = Object(l,m);
  }

}


LODLevel::LODLevel(unsigned int l, glm::vec2& center, Terrain* t) :
  m_Level(l),
  m_Terrain(t),
  m_Size(m_Terrain->GetSize()),
  m_HalfSize(m_Size/2)
{
  //m_UnitSize = m_Size*std::pow(2,(m_NbLevel-m_Level-1));
  m_ActiveR = center - glm::vec2(m_HalfSize);
  m_ClipR = center + glm::vec2(m_HalfSize);
  m_Vertices.resize(m_Size*m_Size-1);

  Load();
  
  

}

void LODLevel::Load(){
  return static_cast<glm::vec2>( chunk*CHUNK_SIZE - chunk );
  glm::vec2 dir = static_cast<glm::i32vec2>(m_ActiveR - m_ClipR);

  glm::i32vec p;
  for(p.y=m_ActiveR.y ; p.y <  ; p.y++){
    for(p.x=m_ActiveR.x ; p.x < toL ; p.x++){
      PutVertex(p, Vertexun(
            glm::vec3(p.x,0,p.y),
            glm::vec2(),
            glm::vec3()
            )
          );
    }
  }
}


void LODLevel::PutVertex(glm::i32vec2& pos, Vertexun v){
  glm::i32vec2 p = pos % m_Size;
  m_Vertices[p.x+p.y*m_Size] = v;
}

void LODLevel::PutVertex(glm::i32vec2& pos, Vertexun v){
  glm::i32vec2 p = pos % m_Size;
  m_Vertices[p.x+p.y*m_Size] = v;
}
