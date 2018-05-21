#include "Terrain.h"
#include "Model.h"
#include "Vertex.h"
#include <GL/glew.h>

Terrain::Terrain(){
  precision = 5.0f;
  last_chunk = glm::i32vec2(0);
  initload(glm::vec2(0.0f, 0.0f)); 
  compute_indices();
  Init(GL_TRIANGLE_STRIP, "shaders/Basic.shader");
}



void Terrain::compute_indices(){
  for(unsigned int k=0;k<CHUNK_PER_SIDE*CHUNK_PER_SIDE;k++){
    for(unsigned int i=0;i<(CHUNK_SIZE * precision)-1;i++){
      for(unsigned int j=0;j<(CHUNK_SIZE * precision);j++){
        m_Indices.push_back((CHUNK_SIZE*precision * CHUNK_SIZE*precision * k) + j + (CHUNK_SIZE*precision * i));
        m_Indices.push_back((CHUNK_SIZE*precision * CHUNK_SIZE*precision * k) + j + (CHUNK_SIZE*precision * (i + 1)));
      }
      m_Indices.push_back(precision*precision*CHUNK_SIZE*CHUNK_SIZE*CHUNK_PER_SIDE*CHUNK_PER_SIDE);
    }
  }
}

void Terrain::initload(glm::vec2 center){
  float offset_x;
  float offset_y = (-1) * RENDER_DISTANCE;
  for(int k=0;k<CHUNK_PER_SIDE;){
    offset_x = (-1) * RENDER_DISTANCE;
    for(int l=0;l<CHUNK_PER_SIDE;){
      buffer_map[k*CHUNK_PER_SIDE+l] = glm::i32vec2(l,k);
      for(float i=0.0f;i<CHUNK_SIZE;i+=(1/precision)){
        for(float j=0.0f;j<CHUNK_SIZE;j+=(1/precision)){
          //printf("x:%f, z:%f \n", j+offset_x, i+offset_y);
          m_Positions.push_back(Vertex( glm::vec3(j + offset_x, noise.compute(j+offset_x,i + offset_y), i + offset_y)));
        }
      }

      l++;
      offset_x = ((-1) * RENDER_DISTANCE) + (l * (CHUNK_SIZE-(1/precision)));
    }
    k++;
    offset_y = ((-1) * RENDER_DISTANCE) + (k * (CHUNK_SIZE-(1/precision)));
  }
}

void Terrain::loadchunk(glm::i32vec2 coords, glm::i32vec2 replace){


    

  // upload
  coords = (coords*CHUNK_SIZE)-coords;
  int vertices_per_chunk = precision*precision*CHUNK_SIZE*CHUNK_SIZE;
  // Make buffer
  Vertex * chunk_to_load = (Vertex*) malloc(sizeof(Vertex) * static_cast<int>(precision*precision)*CHUNK_SIZE*CHUNK_SIZE);

  // Generate data
  unsigned int index1 = 0;
  for(float i=0.0f;i<CHUNK_SIZE;i+=(1/precision)){
    for(float j=0.0f;j<CHUNK_SIZE;j+=(1/precision)){
      chunk_to_load[index1++] = Vertex( glm::vec3(j + coords.x, noise.compute(j+coords.x,i+coords.y), i + coords.y));
    }
  }
  
  int size = vertices_per_chunk * sizeof(Vertex);

  // Locate where it needs to go in the GPU memory
  int offset = getOffset(replace) * size;
  Bind();
  GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, chunk_to_load));

  free(chunk_to_load);
}

int Terrain::getOffset(glm::i32vec2 coords){
  for(unsigned int i; i<CHUNK_PER_SIDE*CHUNK_PER_SIDE;i++){
     if(buffer_map[i] == coords)
      return i;
  } 
  printf("chunk %d %d is not in buffer map \n");
  return -1;
}



void Terrain::load(glm::vec2 coords){
  // round ?
  glm::i32vec2 chunk_coords = static_cast<glm::i32vec2>(coords)/CHUNK_SIZE;
  glm::i32vec2 direction = chunk_coords - last_chunk;

  if(!(direction == glm::i32vec2(0)) ){

    
    glm::i32vec2 add = direction*((CHUNK_PER_SIDE-1)/2+1);
    glm::i32vec2 remove = -direction*((CHUNK_PER_SIDE-1)/2);
    for(unsigned int i; i<CHUNK_PER_SIDE;i++){
      loadchunk(add, remove);
    }

    last_chunk = chunk_coords;
  
  
  }
}


