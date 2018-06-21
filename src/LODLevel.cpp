#include "LODLevel.h"
#include "Debug.h"
#include "Log.h"
unsigned int** LODLevel::pre2D1D;

Mesh<Vertexun> LODLevel::m_Tile;
Mesh<Vertexun> LODLevel::m_Fill[4];
Mesh<Vertexun> LODLevel::m_Trim;
Mesh<Vertexun> LODLevel::m_Cross;
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
  m_UnitSize(glm::pow(2,NB_LEVELS-l-1)),
  m_HeightMap(t->GetHeightMap())
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
  m_NewActiveR = m_ActiveR;


  int size = m_Size+HEIGHT_MAP_EXCESS; // tex coords
  m_HeightMap = new DynamicHeightMap(m_Terrain->GetNoise(),size, m_UnitSize, glm::vec2(1.0f/64.0f), m_NewActiveR-glm::i32vec2(HEIGHT_MAP_EXCESS*m_UnitSize));

  m_Material = new Material(m_HeightMap,m_Terrain->GetShader());

  MakeObjs();

  PlaceTrim();
  //ColorDebug();

}

void LODLevel::ColorDebug(){
  Material* blue = new Material(m_HeightMap, new Shader("shaders/Object.shader") );
  blue->SetKa(glm::vec3(0.0f,0.0f,0.6f));
  blue->Bind();
  blue->SetUniforms();
  Material* lightblue = new Material(m_HeightMap, new Shader("shaders/Object.shader") );
  lightblue->SetKa(glm::vec3(0.0f,0.0f,0.3f));
  lightblue->Bind();
  lightblue->SetUniforms();
  Material* red = new Material(m_HeightMap, new Shader("shaders/Object.shader") );
  red->SetKa(glm::vec3(1.0f,0.0f,0.0f));
  red->Bind();
  red->SetUniforms();
  Material* green = new Material(m_HeightMap, new Shader("shaders/Object.shader") );
  green->SetKa(glm::vec3(0.0f,1.0f,0.0f));
  green->Bind();
  green->SetUniforms();
  Material* yellow = new Material(m_HeightMap, new Shader("shaders/Object.shader") );
  yellow->SetKa(glm::vec3(1.0f,1.0f,0.0f));
  yellow->Bind();
  yellow->SetUniforms();
  bool isFinestLevel = m_Level == m_Terrain->GetNbLevel()-1;
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
  if(isFinestLevel){
    m_CrossObj->m_Materials.clear();
    m_CrossObj->m_Materials.push_back(red);
  }
  return;
}



void LODLevel::PlaceTrim(){

  glm::i32vec2 place = glm::i32vec2(
      (m_NewActiveR.x / m_UnitSize) % 2 == 0,
      (m_NewActiveR.y / m_UnitSize) % 2 == 0
        );
  if(Log::isOn) printf("(lev%d) newActiveR: (%d,%d)\n",m_Level,m_NewActiveR.x,m_NewActiveR.y);
  if(m_Level > 0){
    glm::i32vec2 prev = m_Terrain->GetLods()[m_Level-1]->m_NewActiveR;
    if(Log::isOn) printf("(lev%d) prev newActiveR: (%d,%d)\n",m_Level-1,prev.x,prev.y);
  }
  if(Log::isOn) printf("(lev%d) newActiveR/unitSize: (%d,%d)\n",m_Level,m_NewActiveR.x/m_UnitSize,m_NewActiveR.y/m_UnitSize);
  if(Log::isOn) printf("(lev%d) place: (%d,%d)\n",m_Level,place.x,place.y);
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
  glm::i32vec2 center = m_ActiveR+glm::i32vec2(m_HalfSize*m_UnitSize);
  unsigned int offset = m_TileSize * m_UnitSize;
  glm::i32vec2 pos ;
  for(unsigned int j=0; j < 4; j++){
    pos = center;
    if( j == 0 ){
      pos.x+=offset;
      pos.y-=m_UnitSize;
    }
    else if( j == 1 ){
      pos.y+=offset;
      pos.x-=m_UnitSize;
    }
    else if( j == 2 ){
      pos.x-=offset;
      pos.y-=m_UnitSize;
    }
    else if( j == 3 ){
      pos.y-=offset;
      pos.x-=m_UnitSize;
    }
    MakeFillObj(pos, &m_Fill[j]);
  } 

}

void LODLevel::MakeCrossObj(){
  glm::i32vec2 pos = m_ActiveR+glm::i32vec2(m_HalfSize);
  glm::vec3 p = glm::vec3(pos.x,0,pos.y-1);
  m_CrossObj = new  Object(&m_Cross, m_Material) ;
  m_CrossObj->SetPosition(p);
  m_Objs.push_back(m_CrossObj);
}


void LODLevel::MakeTrimObj(glm::i32vec2& pos, glm::vec3& rot){
  glm::vec3 unitSizeVec = glm::vec3(m_UnitSize);
  glm::vec3 p = glm::vec3(pos.x,0,pos.y);
  m_TrimObj = new  Object(&m_Trim, m_Material) ;
  m_TrimObj->SetPosition(p);
  m_TrimObj->SetScale(unitSizeVec);
  m_TrimObj->SetRotation(rot);
  m_Objs.push_back(m_TrimObj);
}
void LODLevel::MakeSeamObj(){
  glm::vec3 unitSizeVec = glm::vec3(m_UnitSize);
  m_SeamObj = new  Object(&m_Seam, m_Material) ;
  m_SeamObj->SetScale(unitSizeVec);
  m_Objs.push_back(m_SeamObj);
}

void LODLevel::MakeFillObj(glm::i32vec2& pos, Mesh<Vertexun>* m){
  glm::vec3 unitSizeVec = glm::vec3(m_UnitSize);
  glm::vec3 p = glm::vec3(pos.x,0,pos.y);
  m_FillObjs.push_back(new  Object(m, m_Material) );
  m_FillObjs.back()->SetPosition(p);
  m_FillObjs.back()->SetScale(unitSizeVec);
  m_Objs.push_back(m_FillObjs.back());
}

void LODLevel::MakeTileObj(glm::i32vec2& pos){
  glm::vec3 unitSizeVec = glm::vec3(m_UnitSize);
  glm::vec3 p = glm::vec3(pos.x,0,pos.y);
  m_TileObjs.push_back(new  Object(&m_Tile, m_Material) );

  m_TileObjs.back()->SetScale(unitSizeVec);
  m_TileObjs.back()->SetPosition(p);
  m_Objs.push_back(m_TileObjs.back());
}

void LODLevel::MakeObjs(){


  bool isFinestLevel = m_Level == m_Terrain->GetNbLevel()-1;

  MakeTileObjs();
  if(!isFinestLevel)
    MakeFillObjs();
  else
    MakeCrossObj();
  glm::vec3 rot = glm::vec3(0);
  MakeTrimObj(m_ActiveR,rot);
  MakeSeamObj();

}

/* should be done eventually */

/*void LODLevel::Make2D1D(unsigned int s){
  is it already allocated ?
    if(!pre2D1D){
      pre2D1D = (unsigned int**) malloc(sizeof(unsigned int*) * s);
      for(unsigned int i=0; i < s; i++){
        pre2D1D[i] = (unsigned int*) malloc(sizeof(unsigned int) * s);
      }
      if(!pre2D1D){
        if(Log::isOn) printf("[ERROR] Could not allocate");
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
    // we're updating
    if(Log::isOn) printf("(lev %d) [INFO] Updating LOD center: (%d,%d)\n",m_Level,center.x,center.y);
    //if(Log::isOn) printf("(lev %d) terrain center: (%d,%d)\n",m_Level,m_Terrain->m_Center.x,m_Terrain->m_Center.y);
    glm::vec3 dir3 = glm::vec3(dir.x, 0.0f, dir.y);

    glm::i32vec2 d = glm::i32vec2(0);

    // update heightmap
    int q = HEIGHT_MAP_EXCESS*m_UnitSize;
    if(m_NewActiveR.x - m_HeightMap->GetBase().x < q || m_HeightMap->GetBase().x+m_HeightMap->GetWidth()*m_UnitSize - (m_NewActiveR.x + m_Size*m_UnitSize) < q )
      d.x = dir.x;
    if(m_NewActiveR.y - m_HeightMap->GetBase().y < q || m_HeightMap->GetBase().y+m_HeightMap->GetWidth()*m_UnitSize - (m_NewActiveR.y + m_Size*m_UnitSize) < q )
      d.y = dir.y;
    m_HeightMap->Update(d);

    // Move all meshes
    for(unsigned int i = 0 ; i < m_Objs.size() ; i++){
      m_Objs.at(i)->Translate(dir3);
    }
    // Rotate the trim where it needs to go
    PlaceTrim();

    m_ActiveR = m_NewActiveR;
  }

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

  GenCross();
}

void LODLevel::Grid(glm::i32vec2 s, glm::i32vec2 e, Mesh<Vertexun>* m){
  glm::i32vec2 p;
  for(p.y=s.y ; p.y < e.y ; p.y++){
    for(p.x=s.x ; p.x < e.x ; p.x++){
      m->PushVertex( Vertexun(
            glm::vec3( p.x, 0.0f, p.y),
            glm::vec2(),
            glm::vec3(0.0f,1.0f,0.0f)
            )
          );
    }
  }
}

void LODLevel::GenTile(){

  Grid(glm::i32vec2(0), glm::i32vec2(m_TileSize+1), &m_Tile);

}

void LODLevel::GenFill(){

  int i = 0;
  glm::i32vec2 s;
  glm::i32vec2 e;
  m_Fill[i] = {};
  s = glm::i32vec2(0);
  e = glm::i32vec2(m_TileSize+1,2);
  Grid(s, e, &m_Fill[i]);
  m_Fill[i].GridIndices(e);
  i++;

  m_Fill[i] = {};
  s = glm::i32vec2(0);
  e = glm::i32vec2(2,m_TileSize+1);
  Grid(s, e, &m_Fill[i]);
  m_Fill[i].GridIndices(e);
  i++;
  
  m_Fill[i] = {};
  s = glm::i32vec2(-(m_TileSize+1),0);
  e = glm::i32vec2(0,2);
  Grid(s, e, &m_Fill[i]);
  e = glm::i32vec2(m_TileSize+1,2);
  m_Fill[i].GridIndices(e);
  i++;
  
  m_Fill[i] = {};
  s = glm::i32vec2(0,-(m_TileSize+1));
  e = glm::i32vec2(2,0);
  Grid(s, e, &m_Fill[i]);
  e = glm::i32vec2(2,m_TileSize+1);
  m_Fill[i].GridIndices(e);
  i++;
  
  for(i=0 ; i < 4 ; i++){
    m_Fill[i].Init(GL_TRIANGLE_STRIP);
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

void LODLevel::GenCross(){
  m_Cross = {};
  glm::i32vec2 s[3];
  glm::i32vec2 e[3];
  s[0] = glm::i32vec2(-(m_HalfSize),0);
  e[0] = glm::i32vec2(m_HalfSize,2);
  Grid(s[0], e[0], &m_Cross);
  
  s[1] = glm::i32vec2(-1,-(m_HalfSize)+1);
  e[1] = glm::i32vec2(1,1);
  unsigned int a = m_Cross.GetVertices()->size()+1;
  Grid(s[1], e[1], &m_Cross);

  s[2] = glm::i32vec2(-1,0);
  e[2] = glm::i32vec2(1,m_HalfSize+1);
  unsigned int b = m_Cross.GetVertices()->size()+1;
  Grid(s[2], e[2], &m_Cross);


  e[0] = glm::i32vec2(m_Size-1,2);
  m_Cross.GridIndices(e[0]);

  e[1] = glm::i32vec2(2,1);
  m_Cross.GridIndices(e[1],s[1],a);


  e[2] = glm::i32vec2(2,m_HalfSize+1);
  m_Cross.GridIndices(e[2],s[2],b);

  m_Cross.Init(GL_TRIANGLE_STRIP);

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
