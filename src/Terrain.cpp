#include "Terrain.h"
#include "Model.h"
#include "Vertex.h"
#include <vector>
#include <GL/glew.h>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtc/matrix_integer.hpp> 

Terrain::Terrain(glm::vec2 center){
  precision = 4.0f; // Beware of 2
  initload(center); 
  compute_indices();
  Model<Vertexun>::ComputeNormals();
  Init(GL_TRIANGLE_STRIP, "shaders/Terrain.shader");
}



void Terrain::compute_indices(){
  int scale = CHUNK_SIZE*precision -(precision-1.0f);
  for(unsigned int k=0;k<CHUNK_PER_SIDE*CHUNK_PER_SIDE;k++){
    for(unsigned int i=0;i<=(CHUNK_SIZE-1) * precision -1;i++){
      for(unsigned int j=0;j<=(CHUNK_SIZE-1) * precision;j++){
        m_Indices.push_back(j+(i * scale) + scale*scale*k);
        m_Indices.push_back(j+((i+1)*scale) + scale*scale*k);
      }
      m_Indices.push_back(m_Vertices.size());
    }
  }
}


glm::vec2 Terrain::GetPoint(glm::i32vec2 chunk){

  return static_cast<glm::vec2>( chunk*CHUNK_SIZE - chunk );

}

glm::i32vec2 Terrain::Surround(glm::i32vec2 chunk, unsigned int count){

  return chunk + surroundings[count];

}

bool Terrain::IsLoaded(glm::i32vec2 chunk){
  glm::i32vec2 coordsArea = chunk - (last_chunk-CHUNK_PER_SIDE/2);
  return glm::compMin(coordsArea) < 0 || glm::compMax(coordsArea) > CHUNK_PER_SIDE-1;
}

void Terrain::ComputeNormals(glm::i32vec2 chunk_coords, Vertexun * vertices, unsigned int * indices, unsigned int offset){
/*
  // Bottom left point of chunk_coords
  glm::vec2 firstPoint = GetPoint(chunk_coords);

  // Bottom left chunk of loaded area
  glm::i32vec2 firstChunk = (last_chunk-CHUNK_PER_SIDE/2);
  
  // coords of chunk_coords relative to bottom left of loaded area
  glm::i32vec2 coordsArea = chunk_coords - firstChunk;

  indices_per_chunk = 2*pow(2,(CHUNK_SIZE-1)*precision) - 1;

  unsigned int indices_offset = indices_per_chunk*coordsArea.y+coordsArea.x;
  
  std::vector<Vertex> sidesVb;
  std::vector<Vertex> sidesIb;
  glm::i32vec2 side;
  glm::i32vec2 dir;
  for(unsigned int side=0; side<4; i++){                             // for each side of the chunk
      dir = surroundings[side];
      if(glm::compMin(coordsArea+dir) < 0                           
          || glm::comMax(coordsArea+dir) > CHUNK_PER_SIDE-1){        // if there is no chunk loaded on this side
    
        surrChunk = Surround(chunk_coords,side);                     // gotta compute points for that side
        glm::vec2 start = dir*precision;
        for(unsigned int j = 0; j<(CHUNK_SIZE-1)*precision; j++){
          firstPoint+sdir
        }
      
      }
      else{                                     // chunk is loaded
                                                // lets get the relevant points
        
      
      }
      

  }
  Vertexun* surroundingVb;
  //ComputeSurroundingVb(glm::i32vec2& start, surroundingVb);




  unsigned int i1 = m_Indices[0];
  unsigned int i2 = m_Indices[1];
  unsigned int i3;
  glm::vec3 triangle_normal;
  bool order = true;
  for(unsigned int i=2;i<m_Indices.size()-1;i++){
    if(m_Indices[i] == m_Vertices.size()){
      // reached end of line (index == restart index)
      i1 = m_Indices[++i];
      i2 = m_Indices[++i];
      i++;
      order = true;
    }
    i3 = m_Indices[i];
    // compute cross product in right order
    if(order)
      triangle_normal = normalize(cross(m_Vertices[i2].pos-m_Vertices[i1].pos,m_Vertices[i3].pos-m_Vertices[i1].pos));
    else
      triangle_normal = normalize(cross(m_Vertices[i2].pos-m_Vertices[i3].pos,m_Vertices[i1].pos-m_Vertices[i3].pos));
    m_Vertices[i1].normal += triangle_normal;
    m_Vertices[i2].normal += triangle_normal;
    m_Vertices[i3].normal += triangle_normal;
    i1 = i2;
    i2 = i3;
    order = !order;
  }
  for(unsigned int i = 0; i<m_Vertices.size();i++){
    m_Vertices[i].normal = normalize(m_Vertices[i].normal);
  }*/ 

}


glm::i32vec2 Terrain::GetChunk(glm::vec2 coords){
  short signum_x = (coords.x > 0) - (coords.x < 0);
  short signum_y = (coords.y > 0) - (coords.y < 0);
  int p = ((-0.5f) * signum_x * signum_x + (0.5f) * signum_x);
  int r = ((-0.5f) * signum_y * signum_y + (0.5f) * signum_y);
  return  glm::i32vec2(p + coords.x/CHUNK_SIZE, r + coords.y/CHUNK_SIZE);
}

void Terrain::initload(glm::vec2 center){
  last_chunk = GetChunk(center);
  glm::i32vec2 firstChunk = (last_chunk-CHUNK_PER_SIDE/2);
  glm::i32vec2 firstPoint = (firstChunk*CHUNK_SIZE) - firstChunk;
  glm::i32vec2 offset = firstPoint;
  float a,b;
  for(int k=0;k<CHUNK_PER_SIDE;){
    offset.x = firstPoint.x;
    for(int l=0;l<CHUNK_PER_SIDE;){
      buffer_map[k*CHUNK_PER_SIDE+l] = glm::i32vec2(firstChunk.x+l,firstChunk.y+k);
      for(int i=0;i<=(CHUNK_SIZE-1)*precision;i++){
        for(int j=0;j<=(CHUNK_SIZE-1)*precision;j++){
          a = (float)j/precision + offset.x;
          b = (float)i/precision + offset.y;
          m_Vertices.push_back(Vertexun( glm::vec3(a, noise.compute(a,b), b), glm::vec2(), glm::vec3()));
        }
      }

      l++;
      offset.x = firstPoint.x + l * (CHUNK_SIZE-1)/**precision*/;
    }
    k++;
    offset.y = firstPoint.y + k * (CHUNK_SIZE - 1)/**precision*/;
  }
}

void Terrain::loadchunk(glm::i32vec2 coords, glm::i32vec2 replace, std::vector<glm::i32vec2> sides){


  // Bottom left chunk of loaded area
  glm::i32vec2 firstChunk = (last_chunk-CHUNK_PER_SIDE/2);
  
  // coords of chunk_coords relative to bottom left of loaded area
  glm::i32vec2 coordsArea = coords - firstChunk;

  //printf("loading chunk %d %d in place of %d %d\n",coords.x, coords.y,replace.x,replace.y);
  glm::vec2 temp_coords = GetPoint(coords);
  
  unsigned int vPerChunk = (CHUNK_SIZE*precision -(precision-1.0f))* (CHUNK_SIZE*precision -(precision-1.0f));
   
  // Locate where it needs to go in the GPU memory
  unsigned int offset = getOffset(replace);
 // Vertexun * chunk_to_load = (Vertexun*) malloc(sizeof(Vertexun) * static_cast<int>(precision*precision)*CHUNK_SIZE*CHUNK_SIZE);

  unsigned int vOffset = vPerChunk * offset;

  // Generate data
  unsigned int index1 = vOffset;
  float a,b;
  for(int i=0;i<=(CHUNK_SIZE-1)*precision;i++){
    for(int j=0;j<=(CHUNK_SIZE-1)*precision;j++){
      a = (float)j/precision + temp_coords.x;
      b = (float)i/precision + temp_coords.y;
      m_Vertices[index1++] = Vertexun( glm::vec3(a, noise.compute(a,b), b), glm::vec2(),glm::vec3());
    }
  }
  

  // compute normals
  unsigned int iPerChunk = (2*(CHUNK_SIZE*precision -(precision-1.0f))-1) * (CHUNK_SIZE*precision -(precision-1.0f))-1;

  unsigned int iOffset = iPerChunk*offset;

  unsigned int i1 = m_Indices[iOffset];
  unsigned int i2 = m_Indices[iOffset+1];
  unsigned int i3;
  glm::vec3 triangle_normal;
  bool order = true;
  for(unsigned int i=iOffset+2;i<iOffset+iPerChunk-1;i++){
    if(m_Indices[i] == m_Vertices.size()){
      // reached end of line (index == restart index)
      i1 = m_Indices[++i];
      i2 = m_Indices[++i];
      i++;
      order = true;
    }
    i3 = m_Indices[i];
    // compute cross product in right order
    if(order)
      triangle_normal = normalize(cross(m_Vertices[i2].pos-m_Vertices[i1].pos,m_Vertices[i3].pos-m_Vertices[i1].pos));
    else
      triangle_normal = normalize(cross(m_Vertices[i2].pos-m_Vertices[i3].pos,m_Vertices[i1].pos-m_Vertices[i3].pos));
    m_Vertices[i1].normal += triangle_normal;
    m_Vertices[i2].normal += triangle_normal;
    m_Vertices[i3].normal += triangle_normal;
    i1 = i2;
    i2 = i3;
    order = !order;
  }
  for(unsigned int i = vOffset; i<vOffset+vPerChunk;i++){
    m_Vertices[i].normal = normalize(m_Vertices[i].normal);
  } 
  
  
  // compute normals for borders
  for(int s=0;s<sides.size();s++){
    
  }
    

  int size = vPerChunk * sizeof(Vertexun);

   
  //update buffermap
  buffer_map[offset] = coords;
  unsigned int gpuOffset=size*offset;

  // upload
  BindVertexBuffer();
  GLCall(glBufferSubData(GL_ARRAY_BUFFER, gpuOffset, size, &m_Vertices[vOffset]));

}

int Terrain::getOffset(glm::i32vec2 coords){
  for(unsigned int i=0; i<CHUNK_PER_SIDE*CHUNK_PER_SIDE;i++){
     if(buffer_map[i] == coords)
      return i;
  } 
  //printf("[ERROR] Chunk %d %d is not in buffer map \n",coords.x, coords.y);
  return -1;
}

void Terrain::load(glm::vec2 coords){
  // round ?
  glm::i32vec2 chunk_coords = GetChunk(coords);
//  printf("coords: %f %f \n",coords.x, coords.y);
//  printf("chunk_coords: %d %d \n",chunk_coords.x, chunk_coords.y);
  glm::i32vec2 direction = chunk_coords - last_chunk;

  if(!(direction == glm::i32vec2(0,0)) ){
  //printf("last_chunk: %d %d \n",last_chunk.x, last_chunk.y);
  //printf("direction: %d %d \n",direction.x, direction.y);

    std::vector<glm::i32vec2> sides;

    last_chunk = chunk_coords;
    glm::i32vec2 add = direction * (RENDER_DISTANCE);
    glm::i32vec2 remove = direction * (-1)*(RENDER_DISTANCE+1);
    for (int i = -RENDER_DISTANCE; i < RENDER_DISTANCE+1; i++)
      loadchunk(last_chunk + add + glm::i32vec2(direction.y * i, direction.x * i), last_chunk + remove + glm::i32vec2(direction.y * i, direction.x * i),sides);
  
  
  }
}


