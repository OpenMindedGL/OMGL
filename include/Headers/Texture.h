#ifndef Texture_H
#define Texture_H
#pragma once

#include "Shader.h"
#include <string>
#include <vector>

#define TEX_DDS 1
#define TEX_OTHER 2
#define DEFAULT_SAMPLER_NAME "u_DefaultSampler"


class Texture 
{
  protected :
    std::string m_FilePath;
    unsigned int m_RendererID;
    unsigned char* m_LocalBuffer;  // Encoded heights 
    int m_Width, m_Height, m_BPP;
    unsigned int m_Format;
    unsigned int m_Target = GL_TEXTURE_2D;
    std::string m_Name;
    unsigned char m_Slot;

    unsigned int LoadDDS(const std::string& path, unsigned int target, bool genMipMaps = true);
    unsigned int LoadOther(const std::string& path, unsigned int target);
    inline unsigned int LoadDDS(const std::string& path) { LoadDDS(path, GL_TEXTURE_2D); }
    inline unsigned int LoadOther(const std::string& path) { LoadOther(path, GL_TEXTURE_2D); }
    void MakeDDS(bool genMipMaps);
    void MakeOther(bool genMipMaps);
    void Make(unsigned char * buffer);

  public: 

  
    Texture(const std::string& path, std::string name, unsigned char slot, bool genMipMaps = true);
    inline Texture(const std::string& path) : Texture(path, DEFAULT_SAMPLER_NAME, 0) {}
    inline Texture(const std::string& path, std::string name) : Texture(path, name, 0) {}
    inline Texture() : m_Name(DEFAULT_SAMPLER_NAME), m_Slot(0) {}
    Texture(unsigned char * buffer);
    Texture(unsigned int width, unsigned int height = 0);
    ~Texture();

    void Bind(unsigned int slot) const; 
    inline void Bind() const { Bind(m_Slot); }
    void Unbind() const;

    inline void LinkToShader(Shader* shader) { LinkToShader(shader, m_Name, m_Slot); }
    inline void LinkToShader(Shader* shader, unsigned char slot) { LinkToShader(shader, m_Name, slot); }
    void LinkToShader(Shader* shader, const std::string& name, unsigned char slot);

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }

    void SavePng(std::string filepath);

    static unsigned int ParseFormat(const std::string& path);

    // getters
    
    // setters
    inline void SetName(std::string n){ m_Name = n; };
    inline void SetSlot(unsigned char s){ m_Slot = s; };

};

#endif
