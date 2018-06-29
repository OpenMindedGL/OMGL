#include "BiomeHeightMap.h"
#include "NoiseTexture.h"
#include "ProcMixer.h"
#include <tuple>


BiomeHeightMap::BiomeHeightMap( ProcMixer* n, unsigned int width, int texsize, glm::vec2 step , glm::i32vec2 base ) :
  HeightMap(n,width,step,base)
{
  Gen(base,step);
//  Encode();
  ////should be done like that (giving name and slot)
  //m_Texture = new Texture((unsigned char *) &(m_HeightsE[0]),HEIGHTMAP_SAMPLER_NAME,0);
  Make(&(m_Texels[0].x));
  //SavePng("textures/heightmap.png");
}

glm::vec4 BiomeHeightMap::packing0 = glm::vec4(1.0, 255.0, 65025.0, 16581375.0);
glm::vec4 BiomeHeightMap::packing1 = glm::vec4(1.0/255.0,1.0/255.0,1.0/255.0,0.0); 
glm::vec4 BiomeHeightMap::packing2 = glm::vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0); 

glm::vec3 BiomeHeightMap::EncodeFloatRGB( float v) {
  const float max24int = 256.0 * 256.0 * 256.0 - 1.0;
  v *= max24int;
  glm::vec3 result = floor(v / glm::vec3(256.0 * 256.0, 256.0, 1.0));
  result.gb -= result.rg * 256.0;
  result /= 255.0;
  return glm::vec3(result);
}
float BiomeHeightMap::DecodeFloatRGB( glm::vec3 rgb ) {
  const float max24int = 256.0 * 256.0 * 256.0 - 1.0;
  float result = 255.0 * glm::dot(rgb, glm::vec3(256.0 * 256.0, 256.0, 1.0)) / max24int;
  return result;
}

void BiomeHeightMap::Encode(){
  m_Texels.clear();
  for(unsigned int i=0;i<m_HeightsD.size();i++){
    m_Texels.push_back(
        glm::u8vec4(
          static_cast<glm::u8vec3>(
            EncodeFloatRGB( m_HeightsD[i])
            ),
          m_Biomes[i])
        );
  }
}

void BiomeHeightMap::Decode(){
  m_HeightsD.clear();
  for(unsigned int i=0;i<m_Texels.size();i++){
    m_HeightsD.push_back(
        DecodeFloatRGB(
            glm::vec3(m_Texels[i].x,m_Texels[i].y,m_Texels[i].z)
          );
        )
    m_Biomes.push_back(m_Texels[i].w);
  }
}

void BiomeHeightMap::Gen(glm::i32vec2& base, glm::vec2& step) {
  
  printf("(BiomeHeightMap::Gen) m_TexelSize: %d\n",m_TexelSize);
  glm::i32vec2 e = base + glm::i32vec2(m_Width*m_TexelSize);
  std::tuple<float,unsigned int> a;
  for(int i=base.y;i<e.y;i+=m_TexelSize){
    for(int j=base.x;j<e.x;j+=m_TexelSize){
          a = m_Noise->mix((float)j*step.x,(float)i*step.y) // + maxnoise) /maxnoise*2; (mapping to (0,1))
      m_HeightsD.push_back(std::get<0>(a));
      m_Biomes.push_back(std::get<1>(a));
      m_Texels.push_back(
          static_cast<glm::u8vec4>(
            EncodeFloatCharRGBA(
              std::get<0>(a), std::get<1>(a) )
            )
          );
    }
  }
}


//Texture* BiomeHeightMap::MakeNormalMap(){
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
std::tuple<float,unsigned int> BiomeHeightMap::ComputeHeightAndBiome(glm::i32vec2 p){
  return m_Noise->mix((float)p.x*m_Step.x,(float)p.y*m_Step.y); // + maxnoise) /maxnoise*2; (mapping to (0,1))
  // +16/32 should be put inside NoiseGen
}
