#include "LODLevel.h"
#include "Debug.h"
#include "Log.h"
unsigned int** LODLevel::pre2D1D;

Mesh<Vertexun> LODLevel::m_Tile;
Mesh<Vertexun> LODLevel::m_Fill;
Mesh<Vertexun> LODLevel::m_Trim;
Mesh<Vertexun> LODLevel::m_Seam;
int LODLevel::m_Size;
int LODLevel::m_TileSize;
int LODLevel::m_HalfSize;
int LODLevel::m_QuarterSize;
int LODLevel::m_DoubleSize;


// TODO : 
// Make a plane class, use that to make the grids

LODLevel::LODLevel(unsigned int l, glm::vec2& center, Terrain* t) :
  m_Level(l),
  m_Terrain(t),
  m_UnitSize(glm::pow(2,NB_LEVELS-l-1))
{
  if(center.y > 0){
    m_ActiveR.y = glm::floor(center.y / m_UnitSize) * m_UnitSize - m_TileSize*2*m_UnitSize;
  }
  else{
    m_ActiveR.y = glm::floor(center.y)-m_HalfSize * m_UnitSize;
    m_ActiveR.y -= m_NewActiveR.y % m_UnitSize;
  }
  if(center.x > 0){
    m_ActiveR.x = glm::floor(center.x / m_UnitSize) * m_UnitSize - m_TileSize*2*m_UnitSize;
  }
  else{
    m_ActiveR.x = glm::floor(center.x)-m_HalfSize * m_UnitSize;
    m_ActiveR.x -= m_NewActiveR.x % m_UnitSize;
  }
  //m_ActiveR = (static_cast<glm::i32vec2>(center)/glm::i32vec2(m_Size))*glm::i32vec2(m_Size)+glm::i32vec2(m_Size);        // just load everything
  m_NewActiveR = m_ActiveR;

//  m_TorBegin = glm::i32vec2(m_ActiveR.x%(unsigned int)m_Size,m_ActiveR.y%(unsigned int)m_Size);

  MakeObjs();
  PlaceTrim();
  //ColorDebug();
  
  //Update(center);
  //Upload();
  //Log::PrintIndices(*m_Indices);
  //GLCall(Vertexun * a = (Vertexun *) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY)); 
  //printf("%f \n",(*a).pos);
  //GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));

}

void LODLevel::ColorDebug(){
  Material* blue = new Material(new Shader("shaders/Object.shader") );
  blue->SetKa(glm::vec3(0.0f,0.0f,0.6f));
  blue->Bind();
  blue->SetUniforms();
  Material* lightblue = new Material(new Shader("shaders/Object.shader") );
  lightblue->SetKa(glm::vec3(0.0f,0.0f,0.3f));
  lightblue->Bind();
  lightblue->SetUniforms();
  //lightblue->SetD(1);
  Material* red = new Material(new Shader("shaders/Object.shader") );
  red->SetKa(glm::vec3(1.0f,0.0f,0.0f));
  red->Bind();
  red->SetUniforms();
  Material* green = new Material(new Shader("shaders/Object.shader") );
  green->SetKa(glm::vec3(0.0f,1.0f,0.0f));
  green->Bind();
  green->SetUniforms();
  Material* yellow = new Material(new Shader("shaders/Object.shader") );
  yellow->SetKa(glm::vec3(1.0f,1.0f,0.0f));
  yellow->Bind();
  yellow->SetUniforms();
  //red->SetD(1);
  for(unsigned int i=0; i < m_TileObjs.size(); i++){
    m_TileObjs[i]->m_Materials.clear();
    if((i/4)%2 == 0 && i % 2 == 0 || (i/4)%2 != 0 && i % 2 != 0){
      m_TileObjs[i]->m_Materials.push_back(blue);
    }
    else{
      m_TileObjs[i]->m_Materials.push_back(lightblue);
    }
  }
  for(unsigned int i=0; i < m_FillObjs.size(); i++){
      m_FillObjs[i]->m_Materials.clear();
      m_FillObjs[i]->m_Materials.push_back(red);
  }
  m_TrimObj->m_Materials.clear();
  m_TrimObj->m_Materials.push_back(green);
  m_SeamObj->m_Materials.clear();
  m_SeamObj->m_Materials.push_back(yellow);
  return;
}



void LODLevel::PlaceTrim(){

  glm::i32vec2 place = glm::i32vec2(
      (m_NewActiveR.x / m_UnitSize) % 2 == 0,
      (m_NewActiveR.y / m_UnitSize) % 2 == 0
        );
  printf("(lev%d) newActiveR: (%d,%d)\n",m_Level,m_NewActiveR.x,m_NewActiveR.y);
  if(m_Level > 0){
    glm::i32vec2 prev = m_Terrain->m_Lods[m_Level-1]->m_NewActiveR;
    printf("(lev%d) prev newActiveR: (%d,%d)\n",m_Level-1,prev.x,prev.y);
  }
  printf("(lev%d) newActiveR/unitSize: (%d,%d)\n",m_Level,m_NewActiveR.x/m_UnitSize,m_NewActiveR.y/m_UnitSize);
  printf("(lev%d) place: (%d,%d)\n",m_Level,place.x,place.y);
  glm::i32vec2 pos = m_NewActiveR;
  glm::vec3 rot(0);
  glm::vec3 po(m_NewActiveR.x,0.0f,m_NewActiveR.y);
  unsigned int trimLength = (m_Size-1)*m_UnitSize;
  if(place == glm::i32vec2(1,0)){
    rot.y = 3*M_PI/2;
    pos.x += trimLength;
    pos.y-=m_UnitSize;
    po.z-=m_UnitSize;
  }
  else if(place == glm::i32vec2(1,1)){
    rot.y = M_PI;
    pos.x += trimLength;
    pos.y += trimLength;
  }
  else if(place == glm::i32vec2(0,1)){
    rot.y = M_PI/2;
    pos.y += trimLength;
    pos.x-=m_UnitSize;
    po.x-=m_UnitSize;
  }
  else{
    pos.x-=m_UnitSize;
    pos.y-=m_UnitSize;
    po.x-=m_UnitSize;
    po.z-=m_UnitSize;
  }
  /*if(m_NewActiveR.x > 0)
    pos.x+=m_UnitSize;
  if(m_NewActiveR.y > 0)
    pos.y+=m_UnitSize;*/
  glm::vec3 p(pos.x,0.0f,pos.y);
  m_TrimObj->SetPosition(p);
  m_TrimObj->SetRotation(rot);
  m_SeamObj->SetPosition(po);
}



void LODLevel::MakeTileObjs(){

  glm::i32vec2 pos = m_ActiveR;
  bool isFinestLevel = m_Level == m_Terrain->GetNbLevel()-1;
  // Make tile objs
  for(unsigned int i=0; i < 4; i++){
    if(i == 2)
      // 3rd tile is offset by UnitSize to fit the trim
      pos.y+=m_UnitSize;
    for(unsigned int j=0; j < 4; j++){
      if(j == 2)
        // 3rd tile is offset by UnitSize to fit the trim
        pos.x+=m_UnitSize;
      if(isFinestLevel || ( i != 1 && i != 2) || ( j != 1 && j != 2 ) )
        // only make middle tiles for the finest level
        MakeTileObj(pos);
      pos.x+=m_TileSize*m_UnitSize;
    }
    pos.x = m_ActiveR.x;
    pos.y+=m_TileSize*m_UnitSize;
  }


}

void LODLevel::MakeFillObjs(){

  bool isFinestLevel = m_Level == m_Terrain->GetNbLevel()-1;
  glm::i32vec2 pos = m_ActiveR+glm::i32vec2(m_HalfSize*m_UnitSize);
  glm::vec3 rot = glm::vec3(0);
  if(isFinestLevel){
    pos.y--;
    for(unsigned int j=0; j < 4; j++){
      if( j == 1 )
        pos.x--;
      else if( j == 2 )
        pos.y++;
      else if( j == 3 )
        pos.x++;
      MakeFillObj(pos,rot);
      rot.y += M_PI/2;
    }
  }
  unsigned int offset = m_TileSize * m_UnitSize;
  rot = glm::vec3(0);
  pos.x+=offset;
  pos.y-=m_UnitSize;
  for(unsigned int j=0; j < 4; j++){
    if( j == 1 ){
      pos.x-=offset;
      pos.y-=offset;
      pos.x-=m_UnitSize;
    }
    else if( j == 2 ){
      pos.x-=offset;
      pos.y+=offset;
      pos.y+=m_UnitSize;
    }
    else if( j == 3 ){
      pos.x+=offset;
      pos.y+=offset;
      pos.x+=m_UnitSize;
    }
    MakeFillObj(pos,rot);
    rot.y += M_PI/2;
  }

}
void LODLevel::MakeTrimObj(glm::i32vec2& pos, glm::vec3& rot){
  glm::vec3 unitSizeVec = glm::vec3(m_UnitSize);
  glm::vec3 p = glm::vec3(pos.x,0,pos.y);
  m_TrimObj = new  Object(&m_Trim, m_Terrain->GetMaterial()) ;
  m_TrimObj->SetPosition(p);
  m_TrimObj->SetScale(unitSizeVec);
  m_TrimObj->SetRotation(rot);
  m_Objs.push_back(m_TrimObj);
}
void LODLevel::MakeSeamObj(){
  glm::vec3 unitSizeVec = glm::vec3(m_UnitSize);
  m_SeamObj = new  Object(&m_Seam, m_Terrain->GetMaterial()) ;
  m_SeamObj->SetScale(unitSizeVec);
  m_Objs.push_back(m_SeamObj);
}

void LODLevel::MakeFillObj(glm::i32vec2& pos, glm::vec3& rot){
  glm::vec3 unitSizeVec = glm::vec3(m_UnitSize);
  glm::vec3 p = glm::vec3(pos.x,0,pos.y);
  m_FillObjs.push_back(new  Object(&m_Fill, m_Terrain->GetMaterial()) );
  m_FillObjs.back()->SetPosition(p);
  m_FillObjs.back()->SetScale(unitSizeVec);
  m_FillObjs.back()->SetRotation(rot);
  m_Objs.push_back(m_FillObjs.back());
}

void LODLevel::MakeTileObj(glm::i32vec2& pos){
  glm::vec3 unitSizeVec = glm::vec3(m_UnitSize);
  glm::vec3 p = glm::vec3(pos.x,0,pos.y);
  m_TileObjs.push_back(new  Object(&m_Tile, m_Terrain->GetMaterial()) );

  m_TileObjs.back()->SetScale(unitSizeVec);
  m_TileObjs.back()->SetPosition(p);
  m_Objs.push_back(m_TileObjs.back());
}

void LODLevel::MakeObjs(){



  MakeTileObjs();
  MakeFillObjs();
  glm::vec3 rot = glm::vec3(0);
  MakeTrimObj(m_ActiveR,rot);
  MakeSeamObj();

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


/* Updates positions of all meshes
 */
void LODLevel::Update( glm::i32vec2 center ){

    /* just find where the +/-1 is going */ 
    if(center.y > 0){
      m_NewActiveR.y = glm::floor(center.y / m_UnitSize) * m_UnitSize - m_TileSize*2*m_UnitSize;
    }
    else{
      m_NewActiveR.y = glm::floor(center.y)-m_HalfSize * m_UnitSize;
      m_NewActiveR.y -= m_NewActiveR.y % m_UnitSize;
    }
    if(center.x > 0){
      m_NewActiveR.x = glm::floor(center.x / m_UnitSize) * m_UnitSize - m_TileSize*2*m_UnitSize;
    }
    else{
      m_NewActiveR.x = glm::floor(center.x)-m_HalfSize * m_UnitSize;
      m_NewActiveR.x -= m_NewActiveR.x % m_UnitSize;
    }
    /* you lazy bastard */

  glm::i32vec2 dir = m_NewActiveR - m_ActiveR;
  if(dir.x != 0 || dir.y != 0){
    printf("center: (%d,%d)\n",center.x,center.y);
    glm::vec3 dir3 = glm::vec3(dir.x, 0.0f, dir.y);

    // Move all meshes
    for(unsigned int i = 0 ; i < m_Objs.size() ; i++){
      m_Objs.at(i)->Translate(dir3);
    }
    // Rotate the trim where it needs to go
    PlaceTrim();

    m_ActiveR = m_NewActiveR;
    // Update height map ?
    /*if(Load()){
      Upload();
      }
      Unbind();*/
  }
}


int LODLevel::Load(){
  /*  glm::i32vec2 dir = m_NewActiveR - m_ActiveR;

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
*/
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
  /*
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
  */

}

//glm::vec2 dir = static_cast<glm::i32vec2>(m_ActiveR - m_NewActiveR);


void LODLevel::SetOffset( glm::i32vec2& o ){
}

void LODLevel::SetScale(int s){
}

void LODLevel::GenMeshes(unsigned int size){

  m_Size  = size;
  m_HalfSize  = size / 2;
  m_TileSize = (size-3)/4;

  GenTile();
  glm::i32vec2 e = glm::i32vec2(m_TileSize+1);
  m_Tile.GridIndices(e);
  m_Tile.Init(GL_TRIANGLE_STRIP);


  GenFill();
  e = glm::i32vec2(m_TileSize+1,2);
  m_Fill.GridIndices(e);
  m_Fill.Init(GL_TRIANGLE_STRIP);


  GenTrim();
  e = glm::i32vec2(m_Size,2);
  m_Trim.GridIndices(e); 
  e = glm::i32vec2(2,m_Size+1);
  m_Trim.PushIndex(e.y);
  m_Trim.PushIndex((m_Size)*2);
  m_Trim.PushIndex(e.y+1);
  m_Trim.PushIndex((m_Size)*2+1);
  m_Trim.PushIndex(m_Trim.GetVertices()->size());
  m_Trim.GridIndices(e,glm::i32vec2(0,2),(m_Size)*2); 
  m_Trim.Init(GL_TRIANGLE_STRIP);


  GenSeam();
  for(int i=0 ; i < (m_Size-1)*2*3 ;){
    for(int j=0 ; j < 3 ; j++){
      m_Seam.PushIndex(i++);
    }
    i--;
  }
  m_Seam.Init(GL_TRIANGLES);

}

void LODLevel::GenTile(){

  glm::i32vec2 p;
  for(p.y=0 ; p.y < m_TileSize+1 ; p.y++){
    for(p.x=0 ; p.x < m_TileSize+1 ; p.x++){
      m_Tile.PushVertex( Vertexun(
            glm::vec3( p.x, 0.0f, p.y),
            glm::vec2(),
            glm::vec3(0.0f,1.0f,0.0f)
            )
          );
    }
  }

}

void LODLevel::GenFill(){

  glm::i32vec2 p;
  for(p.y=0 ; p.y < 2 ; p.y++){
    for(p.x=0 ; p.x < m_TileSize+1 ; p.x++){
      m_Fill.PushVertex( Vertexun(
            glm::vec3( p.x, 0.0f, p.y),
            glm::vec2(),
            glm::vec3(0.0f,1.0f,0.0f)
            )
          );
    }
  }

}

void LODLevel::GenTrim(){

  glm::i32vec2 p;
  for(p.y=0 ; p.y < 2 ; p.y++){
    for(p.x=0 ; p.x < m_Size ; p.x++){
      m_Trim.PushVertex( Vertexun(
            glm::vec3( p.x, 0.0f, p.y),
            glm::vec2(),
            glm::vec3(0.0f,1.0f,0.0f)
            )
          );
    }
  }
  for(p.y=1 ; p.y < m_Size ; p.y++){
    for(p.x=0 ; p.x < 2 ; p.x++){
      m_Trim.PushVertex( Vertexun(
            glm::vec3( p.x, 0.0f, p.y),
            glm::vec2(),
            glm::vec3(0.0f,1.0f,0.0f)
            )
          );
    }
  }

}

void LODLevel::GenSeam(){

  // could be done more concisely
  // contains the edge of the tile
  for(int a=0 ; a < m_Size-1 ; a++){
    m_Seam.PushVertex( Vertexun(
          glm::vec3( a, 0, 0 ),
          glm::vec2(1,1),
          glm::vec3()
          )
        );
  }
  for(int a=0 ; a < m_Size-1 ; a++){
    m_Seam.PushVertex( Vertexun(
          glm::vec3( m_Size-1, 0, a ),
          glm::vec2(0,0),
          glm::vec3()
          )
        );
  }
  for(int a=m_Size-1 ; a > 0 ; a--){
    m_Seam.PushVertex( Vertexun(
          glm::vec3( a, 0, m_Size-1 ),
          glm::vec2(1,0),
          glm::vec3()
          )
        );
  }
  for(int a=m_Size-1 ; a > 0 ; a--){
    m_Seam.PushVertex( Vertexun(
          glm::vec3( 0, 0, a ),
          glm::vec2(0,1),
          glm::vec3()
          )
        );
  }
}
