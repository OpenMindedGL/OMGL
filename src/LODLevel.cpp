#include "LODLevel.h"
#include "Debug.h"
#include "Log.h"
unsigned int** LODLevel::pre2D1D;


LODLevel::LODLevel(unsigned int l, glm::vec2& center, Terrain* t) :
  m_Level(l),
  m_Terrain(t),
  m_UnitSize(glm::pow(2,NB_LEVELS-l)),
{
  m_ActiveR = (static_cast<glm::i32vec2>(center)/glm::i32vec2(m_Size))*glm::i32vec2(m_Size)+glm::i32vec2(m_Size);        // just load everything

//  m_TorBegin = glm::i32vec2(m_ActiveR.x%(unsigned int)m_Size,m_ActiveR.y%(unsigned int)m_Size);

  MakeObjects();
  Update(center);
  //Upload();
  //Log::PrintIndices(*m_Indices);
  //GLCall(Vertexun * a = (Vertexun *) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY)); 
  //printf("%f \n",(*a).pos);
  //GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));

}

void MakeObjects(){

  unsigned int nboftiles = 12;
  if(m_Level == m_Terrain->m_NbLevels-1)
    nboftiles +=4;               // is finest level

  // Make tile objs
  for(unsigned int i=0; i < nboftiles; i++){
    m_Tiles.push_back(Object(&m_Tile, m_Terrain->m_Material));
    m_Tiles.back().SetScale(glm::vec3(m_UnitSize));
    m_Objs.push_back(m_Tiles.back());
  }

  // Make fill objs
  // Make trim objs
  // Make seam objs

}

/*void LODLevel::Make2D1D(unsigned int s){
  is it already allocated ?
    if(!pre2D1D){
      pre2D1D = (unsigned int**) malloc(sizeof(unsigned int*) * s);
      for(unsigned int i=0; i < s; i++){
        pre2D1D[i] = (unsigned int*) malloc(sizeof(unsigned int) * s);
      }
      if(!pre2D1D){
        printf("[ERROR] Could not allocate");
      }
    }
  fill it
    unsigned int a = 0;
  for(unsigned int i=0; i < s; i++){
    for(unsigned int j=0; j < s; j++){
      pre2D1D[i][j] = a++;
    }
  }
}*/
void LODLevel::GridIndices(glm::i32vec2& e, Mesh<Vertexun>& m){
  glm::i32vec2 ind;
  ind = glm::i32vec2(0);
  int yoffset = 0;      // offset of the line
  int yoffsetn = 0;     // of the next line
  while( ind.y < e.y-1 ){
    yoffsetn+=e.x;
    while( ind.x < e.x ){
      m.m_Indices->push_back(yoffset+ind.x);
      m.m_Indices->push_back(yoffsetn+ind.x);
      ind.x++;
    }
    m.m_Indices->push_back(m.m_Vertices.size());
    ind.y++;
    ind.x = 0;
    yoffset+=e.x;
  }
}

void LODLevel::ComputeIndices(){
}

// Updates positions of all meshes
void LODLevel::Update( glm::i32vec2 center ){
  m_NewActiveR = static_cast<glm::i32vec2>(center)/glm::i32vec2(m_UnitSize) - glm::i32vec2(m_HalfSize);
  glm::i32vec2 dir = m_NewActiveR - m_ActiveR;
  glm::vec3 dir3 = glm::vec3(dir.x, 0.0f, dir.y);

  // Move all meshes
  for(unsigned int i = 0 ; i < m_Objs.size() ; i++){
    m_Objs.at(i).Translate(dir3);
  }

  // Rotate the trim where it needs to go

  // Update height map ?
  /*if(Load()){
    Upload();
  }
  Unbind();*/
}


int LODLevel::Load(){
  glm::i32vec2 dir = m_NewActiveR - m_ActiveR;
  
  // do we update anything ?
  if(dir.x == 0 && dir.y == 0)
    return 0; 

  printf("[INFO] Updating terrain. dir:(%d,%d)\n",dir.x,dir.y);
  // yes
  BindVertexBuffer();
  //MapBuffer();


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
  //GLCall(std::vector<Vertexun> a(m_MappedBuffer,m_MappedBuffer+(*m_Vertices).size()) ); 
  //Log::PrintVertices(a);
  //UnmapBuffer();
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

  if(p.x < 0){
    torPos.x = (glm::abs(p.x)%m_Size);
    if(torPos.x != 0)
      torPos.x=m_Size-torPos.x;
  }
  else{
    torPos.x = p.x%m_Size;
  }

  if(p.y < 0){
    torPos.y = (glm::abs(p.y)%m_Size);
    if(torPos.y != 0)
      torPos.y=m_Size-torPos.y;
  }
  else{
    torPos.y = p.y%m_Size;
  }

 return pre2D1D[torPos.y][torPos.x];
  
}

void LODLevel::PutVertex(glm::i32vec2& pos){

  // index in the VertexBuffer
  unsigned int ind = GetIndex(pos);
  //printf("ind:%d (%d, %d)\n", ind, pos.x, pos.y);
  glm::i32vec2 p =  glm::i32vec2(m_UnitSize*pos.x,m_UnitSize*pos.y);
  // put vertex in
  //m_MappedBuffer[ind] = {
  m_Vertices->at(ind) = {
      //glm::vec3( p.x, 0, p.y),
      glm::vec3( p.x, m_Terrain->m_Noise.compute(p.x*0.06f,p.y*0.06f)*50, p.y),
      glm::vec2(),
      glm::vec3()
  };


}

//glm::vec2 dir = static_cast<glm::i32vec2>(m_ActiveR - m_NewActiveR);


void LODLevel::SetOffset( glm::i32vec2& o ){
}

void LODLevel::SetScale(int s){
}

void LODLevel::GenMeshes(unsigned int size){

  m_Size  = size;
  m_TileSize = (size-3)/4;

  GenTile();
  glm::i32vec2 e = glm::i32vec2(m_TileSize);
  IndicesGrid(e,m_Tile);
  m_Tile.Init(GL_TRIANGLE_STRIP);

    /*
       GenFill();
       GenTrim();
       GenSeam();
       */
}

void LODLevel::GenTile(){

  glm::i32vec2 p;
  for(p.y=0 ; p.y < m_TileSize ; p.y++){
    for(p.x=0 ; p.x < m_TileSize ; p.x++){
      m_Tile.m_Vertices.push_back(
          glm::vec3( p.x, 0.0f, p.y),
          glm::vec2(),
          glm::vec3()
      );
    }
  }

}


void LODLevel::GenFill(){
  
  glm::i32vec2 p;
  for(p.y=0 ; p.y < m_TileSize ; p.y++){
    for(p.x=0 ; p.x < m_TileSize ; p.x++){
      m_Fill.m_Vertices.push_back(
          glm::vec3( p.x, 0.0f, p.y),
          glm::vec2(),
          glm::vec3()
      );
    }
  }

}

void LODLevel::GenTrim(){
  
  glm::i32vec2 p;
  for(p.y=0 ; p.y < m_TileSize ; p.y++){
    for(p.x=0 ; p.x < m_TileSize ; p.x++){
      m_Trim.m_Vertices.push_back(
          glm::vec3( p.x, 0.0f, p.y),
          glm::vec2(),
          glm::vec3()
      );
    }
  }

}

void LODLevel::GenSeam(){
  
  glm::i32vec2 p;
  for(p.y=0 ; p.y < m_TileSize ; p.y++){
    for(p.x=0 ; p.x < m_TileSize ; p.x++){
      m_Seam.m_Vertices.push_back(
          glm::vec3( p.x, 0.0f, p.y),
          glm::vec2(),
          glm::vec3()
      );
    }
  }

}
