#include "Sampler.h"
#include "Debug.h"

Sampler::Sampler(std::string name, unsigned char slot, unsigned int interp, unsigned int wrap) :
  Texture(0,0,name,slot,interp,wrap,false,0)
{
  GLCall(glGenSamplers(1, &m_RendererID));
  GLCall(glSamplerParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, m_Interp));
  GLCall(glSamplerParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, m_Interp));
  GLCall(glSamplerParameteri(m_RendererID, GL_TEXTURE_WRAP_S, m_Wrap));
  GLCall(glSamplerParameteri(m_RendererID, GL_TEXTURE_WRAP_T, m_Wrap));
}


void Sampler::Bind(unsigned int slot) const {
  GLCall(glBindSampler(slot,m_RendererID));
}

Sampler::~Sampler(){
  GLCall(glDeleteSamplers(1,&m_RendererID));
}
