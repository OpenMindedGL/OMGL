#include "Terrain.h"
#include "Model.h"
#include <GL/glew.h>

Terrain::Terrain(){
  initload(glm::vec2(0.0f, 0.0f)); 
  compute_indices();
  Init(GL_TRIANGLE_STRIP, "shaders/Basic.shader");
  //Upload();
}



void Terrain::compute_indices(){
  for(unsigned int k=0;k<NB_CHUNK_PER_SIDE*NB_CHUNK_PER_SIDE;k++){
    for(unsigned int i=0;i<CHUNK_SIZE-1;i++){
      for(unsigned int j=0;j<CHUNK_SIZE;j++){
        m_Indices.push_back((CHUNK_SIZE * CHUNK_SIZE * k) + j + (CHUNK_SIZE * i));
        m_Indices.push_back((CHUNK_SIZE * CHUNK_SIZE * k) + j + (CHUNK_SIZE * (i + 1)));
      }
      m_Indices.push_back(CHUNK_SIZE*CHUNK_SIZE*NB_CHUNK_PER_SIDE*NB_CHUNK_PER_SIDE);
    }
  }
}

void Terrain::initload(glm::vec2 center){
  /*for(int i=center.x-RENDER_DISTANCE;i<RENDER_DISTANCE;i++){
    for(int j=center.y-RENDER_DISTANCE;j<RENDER_DISTANCE;j++){
      m_Positions.push_back(Vertexun(
        glm::vec3(i, noise.compute(i,j)*noise.GetZoom()0,j),
        glm::vec2(0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f))); 
    }
  }*/
  int offset_x;
  int offset_y = -1;
  for(int k=0;k<NB_CHUNK_PER_SIDE;){
    offset_x = -1;
    for(int l=0;l<NB_CHUNK_PER_SIDE;){
      for(int i=0;i<CHUNK_SIZE;i++){
        for(int j=0;j<CHUNK_SIZE;j++){
          printf("x:%d, z:%d \n", j+offset_x, i+offset_y);
          m_Positions.push_back(Vertex( glm::vec3(j + offset_x, 0, i + offset_y)));
        }
      }

      l++;
      offset_x = (-1) + (l * (CHUNK_SIZE-1));
    }
    k++;
    offset_y = (-1) + (k * (CHUNK_SIZE-1));
  }
}

void Terrain::load(glm::vec2 chunkcoords){

}


