#include "Cubemap.h"


Cubemap::Cubemap(){
  faces = {
    "textures/skybox_right.jpg",
    "textures/skybox_left.jpg",
    "textures/skybox_top.jpg",
    "textures/skybox_bottom.jpg",
    "textures/skybox_front.jpg",
    "textures/skybox_back.jpg"
  };
  LoadFaces();
}

void Cubemap::LoadFaces(){
  GLCall(glGenTextures(1, &m_RendererID));
  Bind(); 
  
  for(unsigned int i=0;i<faces.size();i++){
    LoadOther(faces[i],GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}


