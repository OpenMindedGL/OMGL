#include "Terrain.h"
#include "Mesh.h"
#include "Vertex.h"
#include "Debug.h"
#include "Log.h"
#include <vector>
#include "NoiseGen.h"
#include <glm/glm.hpp>



Terrain::Terrain(glm::vec2 spawn, float p, unsigned int s, unsigned int n) :
  m_Precision(p),
  m_NbLevels(n),
  m_Noise()
{
  m_Size = s;

  if((m_Size-1) % 4 != 0){
    printf("[WARNING] trying to make a LODLevel with a size-1 not dividable by 4, bad things gonna happend, you've been warned");
  }
  Material* m_Material = new Material(new Shader("shaders/Terrain.shader"), new Texture());
  m_Material->LoadTexture("textures/rennes.png");
  m_Material->InitTexture("u_HeightMap", 0);

  // ? ComputeNormals();

  LODLevel::GenMeshes(m_Size);   
  LODLevel* l;
  for(unsigned int i=0;i<NB_LEVELS;i++){
    lods[i] = new LODLevel(i,spawn,this);
  }

}


void Terrain::Update(glm::i32vec2& center){

  for(unsigned int i=0;i<NB_LEVELS;i++){
    lods[i]->Update(center);
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



