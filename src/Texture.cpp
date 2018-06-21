#include "Texture.h"
#include "Debug.h"
#include "Shader.h"

#include <string.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" 

Texture::Texture(const std::string& path, std::string name, unsigned char slot, bool genMipMaps) :
  m_FilePath(path),
  m_Name(name),
  m_Slot(slot)
{
  switch (Texture::ParseFormat(path)) {
    case TEX_DDS:
      MakeDDS(genMipMaps);
      break;
    case TEX_OTHER:
      MakeOther(genMipMaps);
      break;
  }

}

Texture::Texture(unsigned int width, unsigned int height) : 
m_Width(width),
m_Name(DEFAULT_SAMPLER_NAME),
m_Slot(0)
{
  if (height == 0)
    m_Height = width;
  else
    m_Height = height;
  
}

void Texture::Make(unsigned char* buffer, unsigned int interp) {
  m_LocalBuffer = buffer;
  GLCall(glGenTextures(1, &m_RendererID));
  Bind();
  /*else if( height ){
    printf("[INFO] Going with a non power of two texture, you're a grown man");
  }*/

  GLCall(glTexImage2D(m_Target, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interp));;
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interp));;
  GLCall(glTexParameteri(m_Target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(m_Target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  Unbind();

  if(m_Height != m_Width){
    printf("[INFO] Going with a rectangle texture, hey I'm not judging\n");
  }
}

Texture::Texture(unsigned char* buffer) {//: m_LocalBuffer(buffer), m_Width(width), m_Name(DEFAULT_SAMPLER_NAME), m_Slot(0) {
  Make(buffer);
}


void Texture::MakeDDS(bool genMipMaps){
  GLCall(glGenTextures(1, &m_RendererID));
  Bind(); 
  LoadDDS(m_FilePath, genMipMaps);

  GLCall(glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 5));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
  GLCall(glTexParameteri(m_Target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(m_Target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
}

void  Texture::MakeOther(bool genMipMaps){
  GLCall(glGenTextures(1, &m_RendererID));
  Bind(); 
  LoadOther(m_FilePath);

  GLCall(glTexParameteri(m_Target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(m_Target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(m_Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  if(genMipMaps){
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glGenerateMipmap(m_Target));
  }
  else{
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));;
  }
}

void Texture::LinkToShader(Shader* shader, const std::string& name, unsigned char slot)
{
	shader->Bind();
	Bind(slot);
	shader->SetUniform1i(name, slot);
	Unbind();
	shader->Unbind();
}

unsigned int Texture::ParseFormat(const std::string& path){
  const char* extension = strrchr(path.c_str(), '.');
  if(!extension ){
    printf("[WARNING] Could not parse extension in filename %s, assuming generic file \n",path);
    return TEX_OTHER;
  }
  else if(strcmp(extension+1, "dds") == 0){
    return TEX_DDS;
  }
  else{
    return TEX_OTHER;
  }

}

unsigned int Texture::LoadOther(const std::string& path, unsigned int target){
  //stbi_set_flip_vertically_on_load(1);
  m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
  if (m_LocalBuffer == NULL){
    printf("[ERROR] Could not load texture file %s\n%s\n",path.c_str(),stbi_failure_reason());
    return 0;
  }
  printf("[INFO] Loaded texture file %s\nwidth:%d height:%d channels:%d\n",path.c_str(),m_Width, m_Height, m_BPP);



  GLCall(glTexImage2D(target, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));


  if (m_LocalBuffer)
    stbi_image_free(m_LocalBuffer);
  // Return the ID of the texture we just created
  return 1;
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(-1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(m_Target, m_RendererID));
}

void Texture::Unbind() const
{
  GLCall(glBindTexture(m_Target, 0));
}


#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

unsigned int Texture::LoadDDS(const std::string& path, unsigned int target, bool genMipMaps){

  const char * imagepath = path.c_str();
  unsigned char header[124];

  FILE *fp; 

  /* try to open the file */ 
  fp = fopen(imagepath, "rb"); 
  if (fp == NULL){
    printf("[ERROR] Could not load texture file %s\n%s\n",path.c_str(),"file not found");
    return 0;
  }

  /* verify the type of file */ 
  char filecode[4]; 
  fread(filecode, 1, 4, fp); 
  if (strncmp(filecode, "DDS ", 4) != 0) { 
    printf("[ERROR] Could not load texture file %s\n%s\n",path.c_str(),"Header doesn't contain 'DDS', m_Formatting it with another tool might help");
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

  if(!genMipMaps){
    mipMapCount = 1;
  }

  unsigned char * buffer;
  unsigned int bufsize;
  /* how big is it going to be including all mipmaps? */ 
  bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize; 
  buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char)); 
  fread(buffer, 1, bufsize, fp); 
  /* close the file pointer */ 
  fclose(fp);

  //unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4; 
  switch(fourCC) 
  { 
    case FOURCC_DXT1: 
      m_Format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; 
      break; 
    case FOURCC_DXT3: 
      m_Format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; 
      break; 
    case FOURCC_DXT5: 
      m_Format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; 
      break; 
    default: 
      free(buffer); 
      printf("[ERROR] Could not load texture file %s\n%s\n",path.c_str(),"Compression type not found in header, compressing it with another tool might help");
      return 0; 
  }

  // "Bind" the newly created texture : all future texture functions will modify this texture
  glBindTexture(m_Target, m_RendererID);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);	

  unsigned int blockSize = (m_Format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
  unsigned int offset = 0;

  /* load the mipmaps */ 
  for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) 
  { 
    unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize; 
    glCompressedTexImage2D(m_Target, level, m_Format, width, height,  
        0, size, buffer + offset); 

    offset += size; 
    width  /= 2; 
    height /= 2; 

    // Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
    if(width < 1) width = 1;
    if(height < 1) height = 1;

  } 

  free(buffer); 

  printf("[INFO] Loaded texture file %s\nwidth:%d height:%d mipmapcount:%d\n",path.c_str(),width, height, mipMapCount);

  return 1;


}



void Texture::SavePng(std::string filepath){
  stbi_write_png(filepath.c_str(), m_Width, m_Width, 4, (const void *) m_LocalBuffer, m_Width*4);
}


