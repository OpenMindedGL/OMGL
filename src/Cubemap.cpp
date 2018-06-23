#include "Debug.h"
#include "Cubemap.h"


Cubemap::Cubemap(std::vector<std::string> faces)
  : m_Faces(faces)
{
  LoadFaces();
}

void Cubemap::LoadFaces(){
  GLCall(glGenTextures(1, &m_RendererID));
  Bind(); 
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  
  for(unsigned int i=0;i<m_Faces.size();i++){
    LoadOther(m_Faces[i]);
    Upload(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
  }
  /*GLCall(glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 6));
  GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));*/

}


