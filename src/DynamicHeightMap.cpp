
#include "DynamicHeightMap.h"

void DynamicHeightMap::UpdateTexel(glm::i32vec2& p, glm::i32vec2& s, glm::i32vec2& t, std::vector<glm::u8vec4>& buffer){
  glm::i32vec2 sp = s+p;
  glm::i32vec2 a = (sp - m_TorBegin) / glm::i32vec2(m_TexelSize);
  glm::i32vec2 tp = GetTorPos(a);
  unsigned int ind = tp.y*m_Width+tp.x;
  if(m_Coarser != NULL && (((unsigned int)sp.x % (m_TexelSize*2)) == 0) && (((unsigned int)sp.y % (m_TexelSize*2) == 0))){
    // has a coarser heightmap
    // and pos was already computed in that previous heightmap
    // can retrieve that value
    //if(m_TexelSize > 127)
      //printf("update without compute %d\n",m_TexelSize);

    glm::i32vec2 g = m_Coarser->GetIndex(sp);
    m_HeightsD[ind] = m_Coarser->GetHeight(g);
    
  }
  else{
    // otherwise just compute it
    m_HeightsD[ind] = ComputeHeight(sp);
  }
  // not necessary, can keep decoded only ones and encode at save time
  m_Texels[ind] =
    static_cast<glm::u8vec4>(
        EncodeFloatRGBA( m_HeightsD[ind] )
        );
  //buffer.push_back(m_Texels[ind]);
}

