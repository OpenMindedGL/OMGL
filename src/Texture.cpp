#include "Texture.h"
#include "Debug.h"
#include "Shader.h"

#include <string.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" 

Texture::Texture( const std::string& path, std::string name, unsigned char slot, unsigned int interp, unsigned int wrap, bool genMipMaps, unsigned int nbmipmaps ) : 
  Texture(0,0,name,slot,interp,wrap,genMipMaps,nbmipmaps)
{
  m_FilePath = path;
  GLCall(glGenTextures(1, &m_RendererID));
  Bind(); 
  switch (Texture::ParseFormat(path)) {
    case TEX_OTHER:
      LoadOther();
      Upload();
      stbi_image_free(m_LocalBuffer);
      break;
    case TEX_DDS:
      LoadDDS();
      break;
  }
  SetParameters();

}

Texture::Texture( unsigned char * buffer, unsigned int width, unsigned int height, std::string name, unsigned char slot, unsigned int interp, unsigned int wrap, bool genMipMaps, unsigned int nbmipmaps ) : 
  Texture(width,height,name,slot,interp,wrap,genMipMaps,nbmipmaps)
{
  Make(buffer);
}


void Texture::SetParameters(){
  Bind();


  GLCall(glTexParameteri(m_Target, GL_TEXTURE_WRAP_S, m_Wrap));
  GLCall(glTexParameteri(m_Target, GL_TEXTURE_WRAP_T, m_Wrap));

  GLCall(glTexParameteri(m_Target, GL_TEXTURE_MAG_FILTER, m_Interp));;

  // mipmaps
  if(m_NbMipMaps != 0){
    // already has them
    GLCall(glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, m_NbMipMaps));
    GLCall(glTexParameteri(m_Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
  }
  else if(m_GenMipMaps){
    // we're asked to generate them
    GLCall(glGenerateMipmap(m_Target));
    GLCall(glTexParameteri(m_Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
  }
  else{
    // doesnt have them, doesnt want them
    GLCall(glTexParameteri(m_Target, GL_TEXTURE_MIN_FILTER, m_Interp));;
  }
  Unbind();

}

void Texture::Make(unsigned char* buffer) {
  m_LocalBuffer = buffer;
  GLCall(glGenTextures(1, &m_RendererID));
  Bind(m_Slot);
  Upload();
  SetParameters();

  if(m_Height != m_Width){
    printf("[INFO] Going with a rectangle texture, hey I'm not judging\n");
  }
}

/*void Texture::MakeDDS(bool genMipMaps){
  LoadDDS(m_FilePath, genMipMaps);

  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
  GLCall(glTexParameteri(m_Target, GL_TEXTURE_WRAP_S, m_Wrap));
  GLCall(glTexParameteri(m_Target, GL_TEXTURE_WRAP_T, m_Wrap));
}

void  Texture::MakeOther(bool genMipMaps){
  GLCall(glGenTextures(1, &m_RendererID));
  Bind(); 

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
}*/

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

unsigned int Texture::LoadOther(const std::string& path){
  //stbi_set_flip_vertically_on_load(1);
  int c;
  m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &c, m_Channels);
  if (m_LocalBuffer == NULL){
    printf("[ERROR] Could not load texture file %s\n%s\n",path.c_str(),stbi_failure_reason());
    return 0;
  }
  printf("[INFO] Loaded texture file %s\nwidth:%d height:%d channels originally:%d channels now:%d\n",path.c_str(),m_Width, m_Height, c, m_Channels);

  return 1;
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(-1, &m_RendererID));
}

void Texture::Upload(unsigned int target, unsigned int inFmt, unsigned int fmt){

  if (!m_LocalBuffer){
    printf("[ERROR] Could not upload texture (%d) %s, local buffer is empty\n",m_RendererID,m_FilePath.c_str());
    return;
  }

  unsigned int inFormat = inFmt;
  unsigned int format = fmt;
  if(m_Channels != 4){
    switch(m_Channels){
      case 3:
        inFormat = GL_RGB8;
        format = GL_RGB;
        break;
      case 2:
        inFormat = GL_RG8;
        format = GL_RG;
        break;
      case 1:
        inFormat = GL_R8;
        format = GL_RED;
        break;
    }
  }
  GLCall(glTexImage2D(target, 0, inFormat, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_LocalBuffer));
  // Return the ID of the texture we just created
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

unsigned int Texture::LoadDDS(const std::string& path, unsigned int target){

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

  if(!m_GenMipMaps){
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


