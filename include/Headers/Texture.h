#ifndef Texture_H
#define Texture_H
#pragma once

#include "Shader.h"
#include <string>
#include <vector>

#define TEX_DDS 1
#define TEX_OTHER 2
#define DEFAULT_SAMPLER_NAME "u_DefaultSampler"
#define DEFAULT_SLOT 0
#define DEFAULT_WRAP_VALUE GL_CLAMP_TO_EDGE
#define DEFAULT_INTERP_VALUE GL_LINEAR
#define DEFAULT_TARGET GL_TEXTURE_2D
#define DEFAULT_NB_MIPMAPS 0
#define DEFAULT_GEN_MIPMAPS false


class Texture 
{
  protected :
    std::string m_FilePath;
    unsigned int m_RendererID;
    unsigned char* m_LocalBuffer;  // Encoded heights 
    int m_Width, m_Height;
    int m_Channels;
    unsigned int m_Format;
    unsigned int m_Target = DEFAULT_TARGET;
    std::string m_Name;
    unsigned char m_Slot;
    unsigned int m_Interp;
    unsigned int m_Wrap;
    bool m_GenMipMaps;  // do we generate mipmaps
    unsigned int m_NbMipMaps; // 0 if has none

    unsigned int LoadDDS(const std::string& path, unsigned int target);
    unsigned int LoadOther(const std::string& path, unsigned int target);

    unsigned int LoadDDS(const std::string& path){ LoadDDS(path,m_Target); }
    unsigned int LoadOther(const std::string& path){ LoadOther(path,m_Target); }

    unsigned int LoadDDS() {  LoadDDS(m_FilePath); }
    unsigned int LoadOther() { LoadOther(m_FilePath); }

    void Make(unsigned char * buffer);
    void SetParameters();

    Texture(
        unsigned int width = 0,
        unsigned int height = 0,
        std::string name = DEFAULT_SAMPLER_NAME,
        unsigned char slot = DEFAULT_SLOT,
        unsigned int interp = DEFAULT_INTERP_VALUE,
        unsigned int wrap = DEFAULT_WRAP_VALUE,
        bool genMipMaps = DEFAULT_GEN_MIPMAPS,
        unsigned int nbmipmaps = DEFAULT_NB_MIPMAPS
        ) :
      m_Width(width),
      m_Height(height == 0 ? width : height),
      m_Name(name),
      m_Slot(slot),
      m_Interp(interp),
      m_Wrap(wrap),
      m_GenMipMaps(genMipMaps),
      m_NbMipMaps(nbmipmaps) { }

  public: 


      Texture(
          const std::string& path,
          std::string name = DEFAULT_SAMPLER_NAME,
          unsigned char slot = DEFAULT_SLOT,
          unsigned int interp = DEFAULT_INTERP_VALUE,
          unsigned int wrap = DEFAULT_WRAP_VALUE,
          bool genMipMaps = DEFAULT_GEN_MIPMAPS,
          unsigned int nbmipmaps = DEFAULT_NB_MIPMAPS
          );

      Texture(
          unsigned char * buffer,
          unsigned int width,
          unsigned int height,
          std::string name = DEFAULT_SAMPLER_NAME,
          unsigned char slot = DEFAULT_SLOT,
          unsigned int interp = DEFAULT_INTERP_VALUE,
          unsigned int wrap = DEFAULT_WRAP_VALUE,
          bool genMipMaps = DEFAULT_GEN_MIPMAPS,
          unsigned int nbmipmaps = DEFAULT_NB_MIPMAPS
          );

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
      unsigned char GetSlot() { return m_Slot; }

    // setters
    inline void SetName(std::string n){ m_Name = n; };
    inline void SetSlot(unsigned char s){ m_Slot = s; };
    inline void SetInterp(unsigned int i){ m_Interp = i; SetParameters(); };
    inline void SetWrap(unsigned int w){ m_Wrap = w; SetParameters();};

};

#endif
