#include "Terrain.h"
#include "Mesh.h"
#include "Vertex.h"
#include <vector>
#include "NoiseGen.h"
#include <glm/glm.hpp>



Terrain:: Terrain(glm::vec2 spawn, float p, unsigned int s, unsigned int n) :
  m_Precision(p),
  m_Size(s),
  m_HalfSize(s/2),
  m_NbLevels(n)
{
  LODLevel::Make2D1D(m_Size);   
  LODLevel* l;
  Material* m = new Material();
  for(unsigned int i=0;i<NB_LEVELS;i++){
    l = new LODLevel(i,spawn,this);
    lods[i] = Object(l,m);
  }

}

unsigned int** LODLevel::pre2D1D;


LODLevel::LODLevel(unsigned int l, glm::vec2& center, Terrain* t) :
  m_Level(l),
  m_Terrain(t),
  m_Size(m_Terrain->GetSize()),
  m_HalfSize(m_Size/2)
{
  //m_UnitSize = m_Size*std::pow(2,(m_NbLevel-m_Level-1));
  m_ActiveR = static_cast<glm::i32vec2>(center) - glm::i32vec2(2*m_Size);        // just load everything
  m_TorBegin = glm::i32vec2(m_ActiveR.x%m_Size,m_ActiveR.y%m_Size);
  //m_ClipR = center + glm::vec2(m_HalfSize);
  m_Vertices->resize(m_Size*m_Size-1);

  Update(center);

  Init(GL_TRIANGLE_STRIP);
  
  

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
    glm::i32vec2 ind;
    glm::i32vec2 indnext;
    indnext.y = m_ActiveR.y;
    glm::i32vec2 end = m_ActiveR + glm::i32vec2(m_Size);
    for(ind.y=m_ActiveR.y; ind.y<end.y-1; ind.y++){
      indnext.y++;
      for(ind.x=m_ActiveR.x; ind.x<end.x; ind.x++){
        m_Indices->push_back(GetIndex(ind));
        m_Indices->push_back(GetIndex(indnext));
      }
      m_Indices->push_back(m_Vertices->size());
    }
}

void LODLevel::Update( glm::i32vec2 center ){
  glm::i32vec2 newActiveR = static_cast<glm::i32vec2>(center) - glm::i32vec2(m_HalfSize);
  glm::i32vec2 dir = m_ActiveR - newActiveR;
  
  // do we update anything ?
  if(dir.x == 0 && dir.y == 0)
    return; 

  // yes

  m_UploadStart = 0;
  m_UploadCount = 0;
  bool isDiagonal;

  // what points need updating ?
  
  // compute area to update
  isDiagonal = false;
  glm::i32vec2 s;                       // start of area
  glm::i32vec2 e;                       // end
  glm::i32vec2& o = m_ActiveR;          // old active region
  glm::i32vec2& n = newActiveR;         // new one
  glm::i32vec2 sign = glm::sign(dir);

  // are any currently loaded points still needed ?
  if (dir.x > m_Size || dir.y > m_Size){
    // no, replace everything
    s = n;
    e = n+glm::i32vec2(m_Size);
  }
  else{
    // yes which ones ?
    if(dir.x != 0 && dir.y != 0)
      isDiagonal == true;

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

  glm::i32vec2 p;
  for(p.y=s.y ; p.y < e.y ; p.y++){
    for(p.x=s.x ; p.x < e.x ; p.x++){
      PutVertex(p);
    }
  }
  
  if(isDiagonal){
    // Second part of the L area  
    e = n+glm::i32vec2(m_Size);
    s.x = 0;
    s.y = p.y;
    if(sign == glm::i32vec2(-1,-1)){
      e.x = o.x;
    }
    else if(sign == glm::i32vec2(1,-1)){
      s.x = o.x;
    }

    glm::i32vec2 p;
    for(p.y=s.y ; p.y < e.y ; p.y++){
      for(p.x=s.x ; p.x < e.x ; p.x++){
        PutVertex(p);
      }
    }
  }

  // upload last range
  Upload(m_UploadStart, m_UploadCount);

  m_ActiveR = newActiveR;
}

int LODLevel::GetIndex(glm::i32vec2& p){
  /* with a modulo
   * glm::i32vec2 torPos = pos % m_Size;
   */

  /* without one
   * /!\ only works if |p-m_ActiveR| < 2*m_Size */

  // TODO : just pass those, increment them in the calling function
  // or declare them globally
  glm::i32vec2 torPos = p - m_ActiveR;
  glm::i32vec2 dir = m_TorBegin + torPos;

  if(dir.x > m_Size)
    torPos.x -= m_Size; 
  else if(dir.x < 0)
    torPos.x += m_Size; 

  if(dir.y > m_Size)
    torPos.y -= m_Size; 
  else if(dir.y < 0)
    torPos.y += m_Size; 
  
  return pre2D1D[torPos.y][torPos.x];
  
}

void LODLevel::PutVertex(glm::i32vec2& pos){

  // index in the VertexBuffer
  unsigned int ind = GetIndex(pos);

  // is this index immediately following others that need uploading ?
  if( ind == 0 || m_UploadStart+m_UploadCount == ind-1){
    // yes, do not upload, maybe next one will follow also
    m_UploadCount++;     
  }
  else{
    // no, upload previous ones
    Upload(m_UploadStart, m_UploadCount);
    // cue this one for later
    m_UploadStart = ind;
  }

  // put vertex in
  m_Vertices->at(ind) = {
      //glm::vec3(pos.x, noise.compute_height(pos.x,pos.y), pos.y),
      glm::vec3(pos.x, 0, pos.y),
      glm::vec2(),
      glm::vec3()
  };


}

//glm::vec2 dir = static_cast<glm::i32vec2>(m_ActiveR - newActiveR);
