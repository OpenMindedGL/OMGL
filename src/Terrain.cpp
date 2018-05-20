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
    for(unsigned int i=0;i<(CHUNK_SIZE * 5)-1;i++){
      for(unsigned int j=0;j<(CHUNK_SIZE*5);j++){
        m_Indices.push_back((CHUNK_SIZE*5 * CHUNK_SIZE*5 * k) + j + (CHUNK_SIZE*5 * i));
        m_Indices.push_back((CHUNK_SIZE*5 * CHUNK_SIZE*5 * k) + j + (CHUNK_SIZE*5 * (i + 1)));
      }
      m_Indices.push_back(5*5*CHUNK_SIZE*CHUNK_SIZE*NB_CHUNK_PER_SIDE*NB_CHUNK_PER_SIDE);
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
  float offset_x;
  float offset_y = -1;
  for(int k=0;k<NB_CHUNK_PER_SIDE;){
    offset_x = -1;
    for(int l=0;l<NB_CHUNK_PER_SIDE;){
      for(float i=0.0f;i<CHUNK_SIZE;i+=0.2f){
        for(float j=0.0f;j<CHUNK_SIZE;j+=0.2f){
          printf("x:%f, z:%f \n", j+offset_x, i+offset_y);
          m_Positions.push_back(Vertex( glm::vec3(j + offset_x, 0, i + offset_y)));
        }
      }

      l++;
      offset_x = (-1) + (l * (CHUNK_SIZE-0.2f));
    }
    k++;
    offset_y = (-1) + (k * (CHUNK_SIZE-0.2f));
  }
}

void Terrain::load(glm::vec2 chunkcoords){

}


