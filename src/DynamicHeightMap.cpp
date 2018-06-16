#include <glm/glm.hpp>

#include "DynamicHeightMap.h"
#include "Debug.h"

void DynamicHeightMap::Upload(glm::i32vec2 offset, glm::i32vec2 size, const void * data){
  GLCall(glTexSubImage2D(
        GL_TEXTURE_2D,          // target
        0,                       // level
        offset.x,          // offsets in the texture
        offset.y,
        size.x,          // size of the sub image
        size.y,
        GL_RGBA,          // GL_RGBA etc
        GL_UNSIGNED_BYTE,            // GL_UNSIGNED_BYTE etc
        data
        )
      );
}



unsigned int DynamicHeightMap::Update(glm::i32vec2 dir){

  // do we update anything ?
  if(dir.x == 0 && dir.y == 0)
    return 0; 

  printf("[INFO] Updating terrain. dir:(%d,%d)\n",dir.x,dir.y);

  bool isDiagonal;
  bool fullReload = false;

  // what points need updating ?

  // compute area to update
  isDiagonal = false;
  glm::i32vec2& o = m_Base;          // old active region
  glm::i32vec2 n = m_Base+dir;         // new one
  glm::i32vec2 s = n;                       // start of area
  glm::i32vec2 e = n+glm::i32vec2(m_Width);                       // end
  glm::i32vec2 sign = glm::sign(dir);

  //printf("old: (%d,%d) new: (%d,%d), tor:(%d,%d)\n",o.x,o.y,n.x,n.y,m_TorBegin.x,m_TorBegin.y);
  // are any currently loaded points still needed ?
  if (glm::abs(dir.x) > m_Width || glm::abs(dir.y) > m_Width){
    // no, replace everything
    fullReload = true;
  }
  else{
    // yes which ones ?
    if(dir.x != 0 && dir.y != 0)
      isDiagonal = true;

    // ugly but fast

    if(sign == glm::i32vec2(1,0)){
      s.x = o.x + m_Width;
    }

    else if(sign == glm::i32vec2(1,1)){
      s.x = o.x + m_Width;
      e.y = o.y;
    }

    else if(sign == glm::i32vec2(0,1)){
      s.y = o.y + m_Width;
    }

    else if(sign == glm::i32vec2(-1,1)){
      e.x = o.x;
      e.y = o.y + m_Width;
    }

    else if(sign == glm::i32vec2(-1,0)){
      e.x = o.x;
    }

    else if(sign == glm::i32vec2(-1,-1)){
      s.x = o.x;
      e.y = o.y;
    }

    else if(sign == glm::i32vec2(0,-1)){
      e.y = o.y;
    }

    else if(sign == glm::i32vec2(1,-1)){
      e.y = o.y;
      e.x = o.x + m_Width;
    }

  }

  printf("1st square: s:(%d,%d), e(%d,%d)\n",s.x,s.y,e.x,e.y);



  if(isDiagonal){
    // Square
    

    // Second Triangle
    e = n+glm::i32vec2(m_Width);
    s.x = n.x;
    s.y = p.y;
    if(sign == glm::i32vec2(-1,-1)){

      e.x = o.x;
    }
    else if(sign == glm::i32vec2(1,-1)){
      s.x = o.x;
    }

    printf("2nd square: s:(%d,%d), e(%d,%d)\n",s.x,s.y,e.x,e.y);
    glm::i32vec2 p;
    for(p.y=s.y ; p.y < e.y ; p.y++){
      for(p.x=s.x ; p.x < e.x ; p.x++){
        //UpdateHeight(p);
      }
    }
  }

  m_Base = n;
  //m_TorBegin = glm::i32vec2(m_ActiveR.x%(unsigned int)m_Width,m_ActiveR.y%(unsigned int)m_Width);
  //return 1;
}

void DynamicHeightMap::UpdateSub(glm::i32vec2& s, glm::i32vec2& e){
  glm::i32vec2 size = e - s + glm::i32vec2(1);  // size of the rectangle
  glm::i32vec2 torOffset = GetTorPos(s);        // bottom left of the rectangle in toroidal indices
  glm::i32vec2 p;
  std::vector<float> toUpdateE;
  toUpdate.clear();
  for(p.y=0 ; p.y < size.y ; p.y++){
    for(p.x=0 ; p.x < size.x ; p.x++){
      UpdateTexel(p, s, torOffset, toUpdateE);
    }
  }
  Upload(torOffset, size, &toUpdate[0]);

}

void DynamicHeightMap::UpdateTexel(glm::i32vec2& p, glm::i32vec2& s,std::vector<float>& buffer){
  
}

int DynamicHeightMap::GetIndex(glm::i32vec2& p){
  glm::i32vec2 torPos = GetTorPos(p);
  return torPos.y*m_Width+torPos.x;
}

glm::i32vec2 GetTorPos(glm::i32vec2& p){
  /* with a modulo
   * glm::i32vec2 torPos = pos % m_Width;
   * x & (m_Width - 1)
   */

  /* without one
   * /!\ only works if |p-m_ActiveR| < 2*m_Width */
  /*
  // TODO : just pass those, increment them in the calling function
  // or declare them globally
  glm::i32vec2 torPos = p - m_ActiveR;
  torPos+=m_TorBegin;
  //glm::i32vec2 dir = m_TorBegin + torPos;
  if(torPos.x < (int) -m_Width)
  torPos.x %= m_Width;
  if(torPos.x >= (int) m_DoubleSize){
  torPos.x %= m_Width;
  }
  else{
  if(torPos.x > (int) m_Width)
  torPos.x -= m_Width; 
  else if(torPos.x < 0)
  torPos.x += m_Width; 
  }
  if(torPos.y < (int) -m_Width)
  torPos.y %= m_Width;
  if(torPos.y >= (int) m_DoubleSize){
  torPos.y %= m_Width;
  }
  else{
  if(torPos.y > (int) m_Width)
  torPos.y -= m_Width; 
  else if(torPos.y < 0)
  torPos.y += m_Width; 
  }
  */

  /* (it's roughly a mod) */

  glm::i32vec2 torPos;
  if(p.x < 0){
    torPos.x = (glm::abs(p.x)%m_Width);
    if(torPos.x != 0)
      torPos.x=m_Width-torPos.x;
  }
  else{
    torPos.x = p.x%m_Width;
  }

  if(p.y < 0){
    torPos.y = (glm::abs(p.y)%m_Width);
    if(torPos.y != 0)
      torPos.y=m_Width-torPos.y;
  }
  else{
    torPos.y = p.y%m_Width;
  }

  return torPos;


}
