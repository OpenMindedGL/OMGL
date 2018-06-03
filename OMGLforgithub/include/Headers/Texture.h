#ifndef Texture_H
#define Texture_H
#pragma once

#include "Renderer.h"
#include <string>

#define TEX_DDS 1
#define TEX_OTHER 2


class Texture 
{
  private :

  protected : 
    std::string m_FilePath;
    unsigned int m_RendererID;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
    static const unsigned int m_Target = GL_TEXTURE_2D;
    unsigned int m_Format;

    //  virtual
    unsigned int Load(const std::string& path, unsigned int target = m_Target){}
    unsigned int LoadDDS(const std::string& path, unsigned int target = m_Target);
    unsigned int LoadOther(const std::string& path, unsigned int target = m_Target);

    Texture(const std::string& path) : m_FilePath(path){}
    Texture(){}
    ~Texture();

  public: 
  

    void Bind(unsigned int slot = 0) const; 
    void Unbind() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }

    static unsigned int ParseFormat(const std::string& path);


};

class DDSTexture : public Texture
{

  protected : 
    void Load(const std::string& path, unsigned int target = m_Target);
  public :
    DDSTexture(const std::string& path);
};

class OtherTexture : public Texture
{

  protected : 
    void Load(const std::string& path, unsigned int target = m_Target);
  public :
    OtherTexture(const std::string& path);
};
#endif
