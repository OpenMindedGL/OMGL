#include "Texture.h"

#include <string.h>
#include "stb_image_aug.h" 

#define TEX_DDS 1
#define TEX_GEN 2

Texture::Texture()
{}

Texture::Texture(const std::string & path, unsigned char type)
	: m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
  switch(type){
    case 0:
      m_RendererID = Load(path);
      break;
    case TEX_DDS:
      m_RendererID = LoadDDS(path);
      break;
    case TEX_GEN:
      m_RendererID = LoadGeneric(path);
      break;

  }
  /*Load(path);
  stbi_set_flip_vertically_on_load(1);
  m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

  GLCall(glGenTextures(1, &m_RendererID));
  GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));

  if (m_LocalBuffer)
    stbi_image_free(m_LocalBuffer);*/
}

unsigned int Texture::Load(const std::string& path){
  const char* extension = strrchr(path.c_str(), '.');
  if(!extension){
    printf("[WARNING] Could not parse extension in filename %s, assuming generic file \n",path);
    Load(path);
  }
  else{
    if(!strcmp(extension+1, "dds"))
      return LoadDDS(path);
    else
      return LoadGeneric(path);
  }
}

unsigned int Texture::LoadGeneric(const std::string& path){
  //stbi_set_flip_vertically_on_load(1);
  //GLCall(glDisable(GL_BLEND));
  m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
  if (m_LocalBuffer == NULL){
    printf("[ERROR] Could not load texture file %s\n%s\n",path.c_str(),stbi_failure_reason());
    return 0;
  }
  printf("[INFO] Loaded texture file %s\nwidth:%d height:%d channels:%d\n",path.c_str(),m_Width, m_Height, m_BPP);
  GLCall(glGenTextures(1, &m_RendererID));

 Bind(); 
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));


  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

  Unbind();

  if (m_LocalBuffer)
    stbi_image_free(m_LocalBuffer);
	// Return the ID of the texture we just created
	return m_RendererID ;
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}





#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

unsigned int Texture::LoadDDS(const std::string& path){

  const char * imagepath = path.c_str();
  unsigned char header[124];

  FILE *fp; 

  /* try to open the file */ 
  fp = fopen(imagepath, "rb"); 
  if (fp == NULL){
    printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); 
    return 0;
  }

  /* verify the type of file */ 
  char filecode[4]; 
  fread(filecode, 1, 4, fp); 
  if (strncmp(filecode, "DDS ", 4) != 0) { 
    printf("not a DDS file \n");
    fclose(fp); 
    return 0; 
  }

  /* get the surface desc */ 
  fread(&header, 124, 1, fp); 

  unsigned int height      = *(unsigned int*)&(header[8 ]);
  unsigned int width	     = *(unsigned int*)&(header[12]);
  unsigned int linearSize	 = *(unsigned int*)&(header[16]);
  unsigned int mipMapCount = *(unsigned int*)&(header[24]);
  unsigned int fourCC      = *(unsigned int*)&(header[80]);


  unsigned char * buffer;
  unsigned int bufsize;
  /* how big is it going to be including all mipmaps? */ 
  bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize; 
  buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char)); 
  fread(buffer, 1, bufsize, fp); 
  /* close the file pointer */ 
  fclose(fp);

  unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4; 
  unsigned int format;
  switch(fourCC) 
  { 
    case FOURCC_DXT1: 
      format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; 
      break; 
    case FOURCC_DXT3: 
      format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; 
      break; 
    case FOURCC_DXT5: 
      format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; 
      break; 
    default: 
      free(buffer); 
      printf("not compressed using DXT1 3 or 5 \n");
      return 0; 
  }

  // Create one OpenGL texture
  GLuint textureID;
  glGenTextures(1, &textureID);

  // "Bind" the newly created texture : all future texture functions will modify this texture
  glBindTexture(GL_TEXTURE_2D, textureID);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);	

  unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
  unsigned int offset = 0;

  /* load the mipmaps */ 
  for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) 
  { 
    unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize; 
    glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,  
        0, size, buffer + offset); 

    offset += size; 
    width  /= 2; 
    height /= 2; 

    // Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
    if(width < 1) width = 1;
    if(height < 1) height = 1;

  } 

  free(buffer); 

  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 5));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  printf("[INFO] Loaded texture file %s\nwidth:%d height:%d mipmapcount:%d\n",path.c_str(),width, height, mipMapCount);

  return textureID;



}




