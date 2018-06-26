
#include "HeightMap.h"



HeightMap::HeightMap(YGen* n, unsigned int width, int texsize, glm::vec2 step,glm::i32vec2 base ) : NoiseTexture(n,width, texsize, step, base)
{
  Gen(base,step);
//  Encode();
  ////should be done like that (giving name and slot)
  //m_Texture = new Texture((unsigned char *) &(m_HeightsE[0]),HEIGHTMAP_SAMPLER_NAME,0);
  Make(&(m_Texels[0].x));
  //SavePng("textures/heightmap.png");
}

glm::vec4 HeightMap::packing0 = glm::vec4(1.0, 255.0, 65025.0, 16581375.0);
glm::vec4 HeightMap::packing1 = glm::vec4(1.0/255.0,1.0/255.0,1.0/255.0,0.0); 
glm::vec4 HeightMap::packing2 = glm::vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0); 

glm::vec4 HeightMap::EncodeFloatRGBA( float v ) {
  glm::vec4 enc = glm::vec4(1.0, 255.0, 65025.0, 16581375.0) * v;
  enc = glm::fract(enc);
  enc -= glm::vec4(enc.y, enc.z, enc.w, enc.w) * glm::vec4(1.0/255.0,1.0/255.0,1.0/255.0,0.0);
  return enc*glm::vec4(256); // probably 255
}
float HeightMap::DecodeFloatRGBA( glm::vec4 rgba ) {
  return glm::dot( rgba, packing2);
}

void HeightMap::Encode(){
  m_Texels.clear();
  for(unsigned int i=0;i<m_HeightsD.size();i++){
    m_Texels.push_back(
        static_cast<glm::u8vec4>(
          EncodeFloatRGBA( m_HeightsD[i] )
          )
        );
  }
}

void HeightMap::Decode(){
  m_HeightsD.clear();
  for(unsigned int i=0;i<m_Texels.size();i++){
    m_HeightsD.push_back(
        DecodeFloatRGBA(
        static_cast<glm::vec4>(
          m_Texels[i])
          )
        );
  }
}

void HeightMap::Gen(glm::i32vec2& base, glm::vec2& step) {
  
  printf("(HeightMap::Gen) m_TexelSize: %d\n",m_TexelSize);
  glm::i32vec2 e = base + glm::i32vec2(m_Width*m_TexelSize);
  for(int i=base.y;i<e.y;i+=m_TexelSize){
    for(int j=base.x;j<e.x;j+=m_TexelSize){
      m_HeightsD.push_back(
          m_Noise->compute((float)j*step.x,(float)i*step.y) // + maxnoise) /maxnoise*2; (mapping to (0,1))
          );
      m_Texels.push_back(
          static_cast<glm::u8vec4>(
            EncodeFloatRGBA( m_HeightsD.back() )
            )
          );
    }
  }
}


//Texture* HeightMap::MakeNormalMap(){
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
//}

// to be put in NoiseTexture eventually
float HeightMap::ComputeHeight(glm::i32vec2 p){
  return m_Noise->compute((float)p.x*m_Step.x,(float)p.y*m_Step.y); // + maxnoise) /maxnoise*2; (mapping to (0,1))
  // +16/32 should be put inside NoiseGen
}
