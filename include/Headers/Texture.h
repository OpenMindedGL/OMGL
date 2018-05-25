#ifndef Texture_H
#define Texture_H
#pragma once

#include "Renderer.h"
#include <string>


class Texture 
{
  private : 
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;

  public: 
    Texture();
    Texture(const std::string & path, unsigned char type=0);
    ~Texture();

    void Bind(unsigned int slot = 0) const; 
    void Unbind() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }

    unsigned int Load(const std::string& path);

    unsigned int LoadDDS(const std::string& path);
    unsigned int LoadGeneric(const std::string& path);


};
#endif
