#include <glm/glm.hpp>

#include "DynamicTexture.h"
#include "Debug.h"

void DynamicTexture::Upload(glm::i32vec2 offset, glm::i32vec2 size, const void * data){
  Bind();
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



unsigned int DynamicTexture::Update(glm::i32vec2 dir){

  // do we update anything ?
  //if(dir.x == 0 && dir.y == 0)
  glm::i32vec2 d = dir;///glm::i32vec2(m_TexelSize);//(dir / glm::i32vec2(2))*glm::i32vec2(2);
  
  if(d.x == 0 && d.y == 0)
    return 0; 

  printf("[INFO] Updating HeightMap. d:(%d,%d)\n",d.x,d.y);
  printf("(DynamicTexture::Update) m_TexelSize: %d\n",m_TexelSize);

  bool isDiagonal;
  bool fullReload = false;

  // what points need updating ?

  // compute area to update
  isDiagonal = false;
  int worldWidth = m_Width * m_TexelSize;
  glm::i32vec2& o = m_Base;          // old active region
  glm::i32vec2 n = m_Base+d;         // new one
  glm::i32vec2 s = n;                       // start of area
  glm::i32vec2 e = n+glm::i32vec2(worldWidth);                       // end
  glm::i32vec2 sign = glm::sign(d);

  //printf("old: (%d,%d) new: (%d,%d), tor:(%d,%d)\n",o.x,o.y,n.x,n.y,m_TorBegin.x,m_TorBegin.y);
  // are any currently loaded points still needed ?
  if (glm::abs(d.x) > worldWidth || glm::abs(d.y) > worldWidth){
    // no, replace everything
    fullReload = true;
  }
  else{
    // yes which ones ?
    if(d.x != 0 && d.y != 0)
      isDiagonal = true;

    // ugly but fast

    if(sign == glm::i32vec2(1,0)){
      s.x = o.x + worldWidth;
    }

    else if(sign == glm::i32vec2(1,1)){
      s.x = o.x + worldWidth;
      e.y = o.y;
    }

    else if(sign == glm::i32vec2(0,1)){
      s.y = o.y + worldWidth;
    }

    else if(sign == glm::i32vec2(-1,1)){
      e.x = o.x;
      e.y = o.y + worldWidth;
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
      e.x = o.x + worldWidth;
    }

  }

  printf("1st rectangle: s:(%d,%d), e(%d,%d)\n",s.x,s.y,e.x,e.y);
  UpdateSub(s,e);

  if(isDiagonal){
    // Square
    

    e = o;
    s = n;
    if(sign == glm::i32vec2(1,1)){
      s += glm::i32vec2(worldWidth);
      e += glm::i32vec2(worldWidth);
    }

    else if(sign == glm::i32vec2(1,-1)){
      s.x = o.x + worldWidth;
      e.x += worldWidth;
    }

    else if(sign == glm::i32vec2(-1,1)){
      s.y = o.y + worldWidth;
      e.y = n.y + worldWidth;
    }
    printf("square: s:(%d,%d), e(%d,%d)\n",s.x,s.y,e.x,e.y);
    UpdateSub(s,e);

    // Second Rectangle
    e = o;
    s = n;
    if(sign == glm::i32vec2(1,1)){
      s.y = o.y + worldWidth;
      e.x += worldWidth;
      e.y = n.y + worldWidth;
    }

    else if(sign == glm::i32vec2(1,-1)){
      s.x = o.x + worldWidth;
      e = n + glm::i32vec2(worldWidth);
    }

    else if(sign == glm::i32vec2(-1,1)){
      s.y = o.y + worldWidth;
      e = n + glm::i32vec2(worldWidth);
    }

    else if(sign == glm::i32vec2(-1,-1)){
      s.y = o.y;
      e.y = n.y + worldWidth;
    }
    printf("2nd rectangle: s:(%d,%d), e(%d,%d)\n",s.x,s.y,e.x,e.y);
    UpdateSub(s,e);
    
  }

  m_Base = n;
  glm::i32vec2 a = (m_Base - m_TorBegin)/glm::i32vec2(m_TexelSize);
  m_TorBase = GetTorPos(a)*glm::i32vec2(m_TexelSize);
  printf("base: (%d,%d), torBase: (%d,%d)\n",m_Base.x,m_Base.y,m_TorBase.x,m_TorBase.y);
  printf("torBegin: (%d,%d)\n",m_TorBegin.x,m_TorBegin.y);
  //m_TorBegin = glm::i32vec2(m_ActiveR.x%(unsigned int)m_Width,m_ActiveR.y%(unsigned int)m_Width);
  //return 1;
  glm::i32vec2 g = glm::i32vec2(m_Width);
  glm::i32vec2 f = glm::i32vec2(0);
  Upload(f, g, &m_Texels[0]);
}

void DynamicTexture::UpdateSub(glm::i32vec2& s, glm::i32vec2& e){
  glm::i32vec2 size = glm::abs(e - s) ;//+ glm::i32vec2(1);  // size of the rectangle
  glm::i32vec2 d = s-m_TorBegin;
  glm::i32vec2 ts = GetTorPos(d);        // bottom left of the rectangle in toroidal indices
  glm::i32vec2 p;
  std::vector<glm::u8vec4> toUpdateE;
  //bool around[2] = false;
  glm::i32vec2 b = size;
//  glm::i32vec2 c = s;
  /*if(offset.x + b.x > m_Width){
    b.x = m_Width - offset.x;
    around[0] = true;
  }
  if(offset.y + size.y > m_Width){
    b.y = m_Width - offset.y;
    around[1] = true;
  }*/
  toUpdateE.clear();
  toUpdateE.resize(size.y*size.x+size.x);
  int inc = (int) m_TexelSize;
  for(p.y=0 ; p.y < b.y ; p.y+=inc){
    for(p.x=0 ; p.x < b.x ; p.x+=inc){
      UpdateTexel(p, s, ts, toUpdateE);
    }
  }
  /*glm::i32vec2 g = glm::i32vec2(m_Width);
  glm::i32vec2 f = glm::i32vec2(0);
  Upload(f, g, &m_Texels[0]);*/
}

void DynamicTexture::UpdateTexel(glm::i32vec2& p, glm::i32vec2& s, glm::i32vec2& t,std::vector<glm::u8vec4>& buffer){
  
}

int DynamicTexture::GetIndex(glm::i32vec2& p){
  glm::i32vec2 torPos = GetTorPos(p);
  return torPos.y*m_Width+torPos.x;
}

glm::i32vec2 DynamicTexture::GetTorPos(glm::i32vec2& p){
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
