#include <tuple>
#include "DynamicBiomeHeightMap.h"

void DynamicBiomeHeightMap::UpdateTexel(glm::i32vec2& p, glm::i32vec2& s, glm::i32vec2& t, std::vector<glm::u8vec4>& buffer){
  glm::i32vec2 sp = s+p;
  glm::i32vec2 a = (sp - m_TorBegin) / glm::i32vec2(m_TexelSize);
  glm::i32vec2 tp = GetTorPos(a);
  unsigned int ind = tp.y*m_Width+tp.x;
  if(m_Coarser != NULL && (((unsigned int)sp.x % (m_TexelSize*2)) == 0) && (((unsigned int)sp.y % (m_TexelSize*2) == 0))){
    // has a coarser heightmap
    // and pos was already computed in that previous heightmap
    // can retrieve that value

    glm::i32vec2 g = m_Coarser->GetIndex(sp);
    m_HeightsD[ind] = m_Coarser->GetHeight(g);
    m_Biomes[ind] = m_Coarser->GetBiome(g);
    
  }
  else{
    // otherwise just compute it
    std::tuple<float,unsigned int> a = ComputeHeightAndBiome(sp);
    m_HeightsD[ind] = std::get<0>(a);
    m_Biomes[ind] = std::get<1>(a);
  }
  m_Texels[ind] = glm::u8vec4(
      static_cast<glm::u8vec3>(
        EncodeFloatRGB( m_HeightsD[ind] )
        ),m_Biomes[ind] 
      );
  //buffer.push_back(m_Texels[ind]);
}

