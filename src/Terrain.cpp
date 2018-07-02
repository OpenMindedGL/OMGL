#include "Terrain.h"
#include "Mesh.h"
#include "Vertex.h"
#include "HeightMap.h"
#include "ProcMixer.h"
#include "UniformBuffer.h"
#include "Biome.h"
#include "Debug.h"
#include "Log.h"
#include <vector>
#include "NoiseGen.h"
#include <glm/glm.hpp>




Terrain::Terrain( std::vector<Biome*>* biomes, ProcMixer* mixer, glm::vec2 spawn, unsigned int s, unsigned int n, float scale) : 
  m_Biomes(biomes),
  m_Mixer(mixer),
  m_Center(static_cast<glm::i32vec2>(spawn)),
  m_Size(s),
  m_NbLevels(n),
  m_Scale(scale)
{
  FillUniformBuffer();

  if((m_Size-3) % 4 != 0){
    printf("[WARNING] trying to make a LODLevel with a size-3 not dividable by 4, bad things gonna happend, you've been warned\n");
  }
  m_Shader = new Shader("shaders/Terrain.shader");

  LODLevel::GenMeshes(m_Size);   
  LODLevel* lp = NULL;
  for(unsigned int i=0;i<m_NbLevels;i++){
    m_Lods[i] = new LODLevel(i,spawn,this,lp);
    lp = m_Lods[i];
  }
  //float* a = new float(0.9);

}

std::vector<YGen*>* Terrain::GetNoises(std::vector<Biome*>* biomes){
  std::vector<YGen*>* a = new std::vector<YGen*>(0);
  for(unsigned int i=0;i<biomes->size();i++){
    a->push_back(biomes->at(i)->m_Height);
  }
  return a;
}

void Terrain::Update(glm::i32vec2& center){

  m_Center = center;

  for(unsigned int i=0;i<NB_LEVELS;i++){
    m_Lods[i]->Update(center);
  }

}


void Terrain::SetUniforms(){
  m_Shader->Bind();
  m_Shader->SetUniform1i("u_MaxHeight",m_Biomes->at(0)->GetMaxHeight()); 
  for(unsigned int i=0;i<m_Biomes->size();i++){
    m_Shader->SetUniform1i("u_MinHeight",m_Biomes->at(0)->GetMinHeight()); 
    m_Shader->SetUniform3f("u_Mat[0].Ka",glm::vec3(0.1));
    m_Shader->SetUniform3f("u_Mat[0].Kd",glm::vec3(0.8));
    m_Shader->SetUniform3f("u_Mat[0].Ks",glm::vec3(0.6));
    m_Shader->SetUniform1f("u_Mat[0].Ns",1);
  }
}

void Terrain::FillUniformBuffer(){
  unsigned int size = Biome::UniformSize * MAX_BIOMES;
  void* p = malloc(size);
  void* o = p;
  unsigned int i;
  for( i=0;i<m_Biomes->size();i++){
    o = m_Biomes->at(i)->FillUniformBuffer(o);
  }
  for(;i<MAX_BIOMES;i++){
    o = Biome::FillEmptyUniform(o);
  }
  m_Ub = new UniformBuffer(p,size);

}
