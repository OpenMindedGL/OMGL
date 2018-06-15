
#include "HeightMap.h"



HeightMap::HeightMap(NoiseGen* n, unsigned int width) : 
  m_Noise(n),
  m_Width(width)  
{
  Gen();
  Encode();
  ////should be done like that
  //m_Texture = new Texture((unsigned char *) &(m_HeightsE[0]),HEIGHTMAP_SAMPLER_NAME,0);
  m_Texture = new Texture((unsigned char *) &(m_HeightsE[0]),width);
  m_Texture->SavePng("textures/heightmap.png");
}

glm::vec4 HeightMap::packing0 = glm::vec4(1.0, 255.0, 65025.0, 16581375.0);
glm::vec4 HeightMap::packing1 = glm::vec4(1.0/255.0,1.0/255.0,1.0/255.0,0.0); 
glm::vec4 HeightMap::packing2 = glm::vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0); 

glm::vec4 HeightMap::EncodeFloatRGBA( float v ) {
  glm::vec4 enc = glm::vec4(1.0, 255.0, 65025.0, 16581375.0) * v;
  enc = glm::fract(enc);
  enc -= glm::vec4(enc.y, enc.z, enc.w, enc.w) * glm::vec4(1.0/255.0,1.0/255.0,1.0/255.0,0.0);
  return enc*glm::vec4(256);
}
float HeightMap::DecodeFloatRGBA( glm::vec4 rgba ) {
  return glm::dot( rgba, packing2);
}

void HeightMap::Encode(){
  m_HeightsE.clear();
  for(unsigned int i=0;i<m_HeightsD.size();i++){
    m_HeightsE.push_back(
        static_cast<glm::u8vec4>(
          EncodeFloatRGBA( m_HeightsD[i] )
          )
        );
  }
}

void HeightMap::Decode(){
  m_HeightsD.clear();
  for(unsigned int i=0;i<m_HeightsE.size();i++){
    m_HeightsD.push_back(
        DecodeFloatRGBA(
        static_cast<glm::vec4>(
          m_HeightsE[i])
          )
        );
  }
}

void HeightMap::Gen() {
  
  for(unsigned int i=0;i<m_Width;i++){
    for(unsigned int j=0;j<m_Width;j++){
      m_HeightsD.push_back((m_Noise->compute((float)j/64,(float)i/64)+25)/50);///100);
    }
  }
}


Texture* HeightMap::MakeNormalMap(){
  //u is one uint size, ie 1.0/texture size
  /*vec2 offsets[4];
  offsets[0] = uv + vec2(-u, 0);
  offsets[1] = uv + vec2(u, 0);
  offsets[2] = uv + vec2(0, -u);
  offsets[3] = uv + vec2(0, u);

  float hts[4];
  for(int i = 0; i < 4; i++)
  {
    hts[i] = dot( texture(u_DefaultSampler, offsets[i]), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) )*4000-2000;
  }

  vec2 _step = vec2(1.0, 0.0);

  vec3 va = normalize( vec3(_step.xy, hts[1]-hts[0]) );
  vec3 vb = normalize( vec3(_step.yx, hts[3]-hts[2]) );

  return cross(va,vb).rbg; //you may not need to swizzle the normal*/
}

