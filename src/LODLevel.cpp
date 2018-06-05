#include "LODLevel.h"
#include "Debug.h"
#include "Log.h"
unsigned int** LODLevel::pre2D1D;


LODLevel::LODLevel(unsigned int l, glm::vec2& center, Terrain* t) :
  m_Level(l),
  m_Terrain(t),
  m_Size(t->GetSize()),
  m_UnitSize(glm::pow(2,NB_LEVELS-l)),
  //m_UnitSize(1),
  m_HalfSize(m_Size/2),
  m_DoubleSize(m_Size*2)
{
  //m_UnitSize = m_Size*std::pow(2,(m_NbLevel-m_Level-1));
  m_ActiveR = (static_cast<glm::i32vec2>(center)/glm::i32vec2(m_Size))*glm::i32vec2(m_Size)+glm::i32vec2(m_Size);        // just load everything
  m_TorBegin = glm::i32vec2(m_ActiveR.x%(unsigned int)m_Size,m_ActiveR.y%(unsigned int)m_Size);
  //m_ClipR = center + glm::vec2(m_HalfSize);
  m_Vertices->resize((unsigned int)m_Size*m_Size);
  m_Indices->resize(2*m_Size*(m_Size-1)+m_Size);

  m_NewActiveR = static_cast<glm::i32vec2>(center)/glm::i32vec2(m_UnitSize) - glm::i32vec2(m_HalfSize);

  Init(GL_TRIANGLE_STRIP);
  Update(center);
  //Upload();
  //Log::PrintIndices(*m_Indices);
  //GLCall(Vertexun * a = (Vertexun *) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY)); 
  //printf("%f \n",(*a).pos);
  //GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));

}

void LODLevel::Make2D1D(unsigned int s){
  // is it already allocated ?
  if(!pre2D1D){
    pre2D1D = (unsigned int**) malloc(sizeof(unsigned int*) * s);
    for(unsigned int i=0; i < s; i++){
      pre2D1D[i] = (unsigned int*) malloc(sizeof(unsigned int) * s);
    }
    if(!pre2D1D){
      printf("[ERROR] Could not allocate");
    }
  }
  // fill it
  unsigned int a = 0;
  for(unsigned int i=0; i < s; i++){
    for(unsigned int j=0; j < s; j++){
      pre2D1D[i][j] = a++;
    }
  }
}

void LODLevel::ComputeIndices(){
  m_Indices->clear();
  glm::i32vec2 ind;
  glm::i32vec2 indnext;
  //if(m_Level == NB_LEVELS){
    indnext.y = m_NewActiveR.y;
    glm::i32vec2 end = m_NewActiveR + glm::i32vec2(m_Size);
    for(ind.y=m_NewActiveR.y; ind.y<end.y-1; ind.y++){
      indnext.y++;
      indnext.x=m_NewActiveR.x;
      for(ind.x=m_NewActiveR.x; ind.x<end.x; ind.x++){
        m_Indices->push_back(GetIndex(ind));
        m_Indices->push_back(GetIndex(indnext));
        indnext.x++;
      }
      m_Indices->push_back(m_Vertices->size());
    }
  }
  else{
    indnext.y = m_NewActiveR.y;
    glm::i32vec2 end = m_NewActiveR + glm::i32vec2(m_Size);
    for(ind.y=m_NewActiveR.y; ind.y<end.y-1; ind.y++){
      indnext.y++;
      indnext.x=m_NewActiveR.x;
      for(ind.x=m_NewActiveR.x; ind.x<end.x; ind.x++){
        m_Indices->push_back(GetIndex(ind));
        m_Indices->push_back(GetIndex(indnext));
        indnext.x++;
      }
      m_Indices->push_back(m_Vertices->size());
    }
  }
}

void LODLevel::Update( glm::i32vec2 center ){
  m_NewActiveR = static_cast<glm::i32vec2>(center)/glm::i32vec2(m_UnitSize) - glm::i32vec2(m_HalfSize);
  if(Load()){
    ComputeIndices();
    Log::PrintIndices(*m_Indices);
    UploadIndexBuffer();
  }
  Unbind();
}


int LODLevel::Load(){
  glm::i32vec2 dir = m_NewActiveR - m_ActiveR;
  
  // do we update anything ?
  if(dir.x == 0 && dir.y == 0)
    return 0; 

  printf("[INFO] Updating terrain. dir:(%d,%d)\n",dir.x,dir.y);
  // yes
  BindVertexBuffer();
  MapBuffer();


  m_UploadStart = 0;
  m_UploadCount = 0;
  bool isDiagonal;
  bool fullReload = false;

  // what points need updating ?
  
  // compute area to update
  isDiagonal = false;
  glm::i32vec2& o = m_ActiveR;          // old active region
  glm::i32vec2& n = m_NewActiveR;         // new one
  glm::i32vec2 s = n;                       // start of area
  glm::i32vec2 e = n+glm::i32vec2(m_Size);                       // end
  glm::i32vec2 sign = glm::sign(dir);

  printf("old: (%d,%d) new: (%d,%d), tor:(%d,%d)\n",o.x,o.y,n.x,n.y,m_TorBegin.x,m_TorBegin.y);
  // are any currently loaded points still needed ?
  if (glm::abs(dir.x) > m_Size || glm::abs(dir.y) > m_Size){
    // no, replace everything
    fullReload = true;
  }
  else{
    // yes which ones ?
    if(dir.x != 0 && dir.y != 0)
      isDiagonal = true;

    // ugly but fast

    if(sign == glm::i32vec2(1,0)){
      s.x = o.x + m_Size;
    }

    else if(sign == glm::i32vec2(1,1)){
      s.x = o.x + m_Size;
      e.y = o.y+m_Size;
    }

    else if(sign == glm::i32vec2(0,1)){
      s.y = o.y + m_Size;
    }

    else if(sign == glm::i32vec2(-1,1)){
      e.x = o.x;
      e.y = o.y+m_Size;
    }

    else if(sign == glm::i32vec2(-1,0)){
      e.x = o.x;
    }

    else if(sign == glm::i32vec2(-1,-1)){
      e.y = o.y;
    }

    else if(sign == glm::i32vec2(0,-1)){
      e.y = o.y;
    }

    else if(sign == glm::i32vec2(1,-1)){
      e.y = o.y;
    }

  }

  printf("1st square: s:(%d,%d), e(%d,%d)\n",s.x,s.y,e.x,e.y);
  glm::i32vec2 p;
  for(p.y=s.y ; p.y < e.y ; p.y++){
    for(p.x=s.x ; p.x < e.x ; p.x++){
      PutVertex(p);
    }
  }

  if(isDiagonal){
    // Second part of the L area  
    e = n+glm::i32vec2(m_Size);
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
        PutVertex(p);
      }
    }
  }

  m_ActiveR = m_NewActiveR;
  m_TorBegin = glm::i32vec2(m_ActiveR.x%(unsigned int)m_Size,m_ActiveR.y%(unsigned int)m_Size);
  GLCall(std::vector<Vertexun> a(m_MappedBuffer,m_MappedBuffer+(*m_Vertices).size()) ); 
  //Log::PrintVertices(a);
  UnmapBuffer();
  return 1;
}

int LODLevel::GetIndex(glm::i32vec2& p){
  /* with a modulo
   * glm::i32vec2 torPos = pos % m_Size;
   * x & (m_Size - 1)
   */

  /* without one
   * /!\ only works if |p-m_ActiveR| < 2*m_Size */
/*
  // TODO : just pass those, increment them in the calling function
  // or declare them globally
  glm::i32vec2 torPos = p - m_ActiveR;
  torPos+=m_TorBegin;
  //glm::i32vec2 dir = m_TorBegin + torPos;
  if(torPos.x < (int) -m_Size)
    torPos.x %= m_Size;
  if(torPos.x >= (int) m_DoubleSize){
    torPos.x %= m_Size;
  }
  else{
    if(torPos.x > (int) m_Size)
      torPos.x -= m_Size; 
    else if(torPos.x < 0)
      torPos.x += m_Size; 
  }
  if(torPos.y < (int) -m_Size)
    torPos.y %= m_Size;
  if(torPos.y >= (int) m_DoubleSize){
    torPos.y %= m_Size;
  }
  else{
    if(torPos.y > (int) m_Size)
      torPos.y -= m_Size; 
    else if(torPos.y < 0)
      torPos.y += m_Size; 
  }
  */

  glm::i32vec2 torPos;
  if(p.x < 0)
    torPos.x = m_Size - (glm::abs(p.x)%m_Size);
  else
    torPos.x = p.x%m_Size;
  if(p.y < 0)
    torPos.y = m_Size - (glm::abs(p.y)%m_Size);
  else
    torPos.y = p.y%m_Size;

 return pre2D1D[torPos.y][torPos.x];
  
}

void LODLevel::PutVertex(glm::i32vec2& pos){

  // index in the VertexBuffer
  unsigned int ind = GetIndex(pos);
  //printf("ind:%d (%d, %d)\n", ind, pos.x, pos.y);
  glm::i32vec2 p =  glm::i32vec2(m_UnitSize*pos.x,m_UnitSize*pos.y);
  // put vertex in
  m_MappedBuffer[ind] = {
      //glm::vec3(pos.x, noise.compute_height(pos.x,pos.y), pos.y),
      glm::vec3( p.x, 0, p.y),
      //glm::vec3( p.x, m_Terrain->m_Noise.compute(p.x,p.y), p.y),
      glm::vec2(),
      glm::vec3()
  };


}

//glm::vec2 dir = static_cast<glm::i32vec2>(m_ActiveR - m_NewActiveR);

void LODLevel::UnmapBuffer(){
  bool a;
  GLCall(a = glUnmapBuffer(GL_ARRAY_BUFFER));
  if(!a)
    printf("[ERROR] UnmapBuffer returned GL_FALSE\n");
}
void LODLevel::MapBuffer(){
  m_MappedBuffer = (Vertexun*) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
}
