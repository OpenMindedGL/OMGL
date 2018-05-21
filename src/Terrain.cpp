#include "Terrain.h"
#include "Model.h"
#include "Vertex.h"
#include <GL/glew.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_integer.hpp> 

Terrain::Terrain(){
  precision = 2.0f;
  last_chunk = glm::i32vec2(1);
  initload(glm::vec2(0.0f, 0.0f)); 
  compute_indices();
 // ComputeNormals();
  Init(GL_TRIANGLE_STRIP, "shaders/Basic.shader");
}



void Terrain::compute_indices(){
  int scale = CHUNK_SIZE*precision -1;
  for(unsigned int k=0;k<CHUNK_PER_SIDE*CHUNK_PER_SIDE;k++){
    for(unsigned int i=0;i<=(CHUNK_SIZE-1) * precision -1;i++){
      for(unsigned int j=0;j<=(CHUNK_SIZE-1) * precision;j++){
        m_Indices.push_back(j+(i * scale) + scale*scale*k);
        m_Indices.push_back(j+((i+1)*scale) + scale*scale*k);
      }
      m_Indices.push_back(m_Positions.size());
    }
  }
}

void Terrain::initload(glm::vec2 center){
  float offset_x;
  float offset_y = 0;
  float a,b;
  for(int k=0;k<CHUNK_PER_SIDE;){
    offset_x = 0;
    for(int l=0;l<CHUNK_PER_SIDE;){
      buffer_map[k*CHUNK_PER_SIDE+l] = glm::i32vec2(l,k);
      for(int i=0;i<=(CHUNK_SIZE-1)*precision;i++){
        for(int j=0;j<=(CHUNK_SIZE-1)*precision;j++){
          a = (float)j/precision + offset_x;
          b = (float)i/precision + offset_y;
          m_Positions.push_back(Vertexun( glm::vec3(a, noise.compute(a,b), b), glm::vec2(), glm::vec3()));
        }
      }

      l++;
      offset_x = l * (CHUNK_SIZE-1)/**precision*/;
    }
    k++;
    offset_y = k * (CHUNK_SIZE - 1)/**precision*/;
  }
}

void Terrain::loadchunk(glm::i32vec2 coords, glm::i32vec2 replace){


    

  printf("loading chunk %d %d in place of %d %d\n",coords.x, coords.y,replace.x,replace.y);
  // upload
  glm::i32vec2 temp_coords = (coords*CHUNK_SIZE)-coords;
  int vertices_per_chunk = (CHUNK_SIZE*precision -1)* (CHUNK_SIZE*precision -1);
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
  m_Vb->Bind();
  GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, chunk_to_load));

}

int Terrain::getOffset(glm::i32vec2 coords){
  for(unsigned int i=0; i<CHUNK_PER_SIDE*CHUNK_PER_SIDE;i++){
     if(buffer_map[i] == coords)
      return i;
  } 
  printf("chunk %d %d is not in buffer map \n");
  return -1;
}



void Terrain::load(glm::vec2 coords){
  // round ?
  glm::i32vec2 chunk_coords = static_cast<glm::i32vec2>(coords)/CHUNK_SIZE;
  //printf("chunk_coords: %d %d \n",chunk_coords.x, chunk_coords.y);
  printf("last_chunk: %d %d \n",last_chunk.x, last_chunk.y);
  glm::i32vec2 direction = chunk_coords - last_chunk;

  printf("direction: %d %d \n",direction.x, direction.y);
  //printf("rot: %d %d \n",(glm::imat2(0,1,-1,0)*direction).x,(glm::imat2(0,1,-1,0)*direction).y);


  if(!(direction == glm::i32vec2(0,0)) ){

    /*glm::i32vec2 add = last_chunk-(glm::i32vec2)(glm::imat2(0,1,-1,0)*direction)*(CHUNK_PER_SIDE/2) + direction*((CHUNK_PER_SIDE/2)+1);
    glm::i32vec2 remove = last_chunk-(glm::i32vec2)(glm::imat2(0,1,-1,0)*direction)*(CHUNK_PER_SIDE/2)+direction*(CHUNK_PER_SIDE/2);
    for(int i=0; i<CHUNK_PER_SIDE;i++){
      loadchunk(last_chunk+add+(glm::i32vec2)(glm::imat2(0,-1,1,0)*direction)*i, last_chunk+remove+(glm::i32vec2)(glm::imat2(0,-1,1,0)*direction)*i);
    }*/

    glm::i32vec2 add = direction * 2;
    glm::i32vec2 remove = direction * (-1);
    for (int i = -1; i < CHUNK_PER_SIDE -1; i++)
      loadchunk(last_chunk + add + glm::i32vec2(direction.y * i, direction.x * i), last_chunk + remove + glm::i32vec2(direction.y * i, direction.x * i));
    last_chunk = chunk_coords;
  
  
  }
}


