#include "ProcTexture.h"

ProcTexture::ProcTexture(NoiseGen* n, unsigned int width, glm::vec2 step,glm::i32vec2 base ) : NoiseTexture(n,width,step,base)
{
  Gen(base,step);
  ////should be done like that (giving name and slot)
  //m_Texture = new Texture((unsigned char *) &(m_HeightsE[0]),HEIGHTMAP_SAMPLER_NAME,0);
  Make(&(m_Texels[0].x));
  //m_Texture->SavePng("textures/heightmap.png");
}

void ProcTexture::Gen(glm::i32vec2& base, glm::vec2& step) {
  
  glm::i32vec2 e = base + glm::i32vec2(m_Width);
  float a;
  for(int i=base.y;i<e.y;i++){
    for(int j=base.x;j<e.x;j++){
      a = (m_Noise->compute((float)j*step.x,(float)i*step.y)+16)/32;
      m_Texels.push_back(static_cast<glm::u8vec4>(m_Color * a)); // + maxnoise) /maxnoise*2; (mapping to (0,1))
        
    }
  }
}
