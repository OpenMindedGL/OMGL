
#include "DynamicHeightMap.h"

void DynamicHeightMap::UpdateTexel(glm::i32vec2& p, glm::i32vec2& s, glm::i32vec2& t, std::vector<glm::u8vec4>& buffer){
  glm::i32vec2 tp = t+p;
  glm::i32vec2 ts = t+s;
  unsigned int ind = tp.y*m_Width+tp.x;
  m_HeightsD[ind] = GetHeight(ts);
  // not necessary, can keep decoded only ones and encode at save time
  m_Texels[ind] =
    static_cast<glm::u8vec4>(
        EncodeFloatRGBA( m_HeightsD[ind] )
        );
  buffer.push_back(m_Texels[ind]);
}

