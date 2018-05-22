#include "Terrain.h"
#include "Model.h"
#include "Vertex.h"
#include <GL/glew.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_integer.hpp> 

Terrain::Terrain(glm::vec2 center){
  precision = 5.0f;
  initload(center); 
  compute_indices();
  ComputeNormals();
  Init(GL_TRIANGLE_STRIP, "shaders/Terrain.shader");
}



void Terrain::compute_indices(){
  int scale = CHUNK_SIZE*precision -(precision - 1.0f);
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

void Terrain::loadchunk(glm::i32vec2 coords, glm::i32vec2 replace){

  //printf("loading chunk %d %d in place of %d %d\n",coords.x, coords.y,replace.x,replace.y);
  // upload
  glm::i32vec2 temp_coords = (coords*CHUNK_SIZE)-coords;
  int vertices_per_chunk = (CHUNK_SIZE*precision - (precision -1.0f))* (CHUNK_SIZE*precision -(precision -1.0f));
  // Make buffer
  Vertexun * chunk_to_load = (Vertexun*) malloc(sizeof(Vertexun) * static_cast<int>(precision*precision)*CHUNK_SIZE*CHUNK_SIZE);

  // Generate data
  unsigned int index1 = 0;
  float a,b;
  for(int i=0;i<=(CHUNK_SIZE-1)*precision;i++){
    for(int j=0;j<=(CHUNK_SIZE-1)*precision;j++){
      a = (float)j/precision + temp_coords.x;
      b = (float)i/precision + temp_coords.y;
      chunk_to_load[index1++] = Vertexun( glm::vec3(a, noise.compute(a,b), b), glm::vec2(),glm::vec3());
    }
  }
  
  int size = vertices_per_chunk * sizeof(Vertexun);

  // Locate where it needs to go in the GPU memory
  int offset = getOffset(replace);
   
    //update buffermap
   
  buffer_map[offset] = coords;
  offset*=size;
  BindVertexBuffer();
  GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, chunk_to_load));

}

int Terrain::getOffset(glm::i32vec2 coords){
  for(unsigned int i=0; i<CHUNK_PER_SIDE*CHUNK_PER_SIDE;i++){
     if(buffer_map[i] == coords)
      return i;
  } 
  printf("[ERROR] Chunk %d %d is not in buffer map \n",coords.x, coords.y);
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

    glm::i32vec2 add = direction * (RENDER_DISTANCE+1);
    glm::i32vec2 remove = direction * (-1)*RENDER_DISTANCE;
    for (int i = -RENDER_DISTANCE; i < RENDER_DISTANCE+1; i++)
      loadchunk(last_chunk + add + glm::i32vec2(direction.y * i, direction.x * i), last_chunk + remove + glm::i32vec2(direction.y * i, direction.x * i));
    last_chunk = chunk_coords;
  
  
  }
}


