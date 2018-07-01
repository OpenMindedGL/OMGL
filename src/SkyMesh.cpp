#include "SkyMesh.h"
#include "Mesh.h"
#include "Vertex.h"
#include <vector>
#include <GL/glew.h>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtc/matrix_integer.hpp> 

SkyMesh::SkyMesh(glm::vec2 center, YGen* rectoNoise, YGen* versoNoise){
  precision = 3.0f; // Beware of 2

  m_VNoise = versoNoise;
  m_RNoise = rectoNoise;

  initload_sky(center); 
  compute_indices_sky();
  compute_normals();
  Init(GL_TRIANGLE_STRIP);
}

void SkyMesh::compute_normals(){
  unsigned int i1 = m_Indices->at(0);
  unsigned int i2 = m_Indices->at(1);
  unsigned int i3;
  glm::vec3 triangle_normal;
  bool order = true;
  for(unsigned int i=2;i<m_Indices->size()-1;i++){
    while ((i1 == m_Vertices->size()) || (i2 == m_Vertices->size()) || ((*m_Indices)[i] == m_Vertices->size())){
      i++;
      i1 = (*m_Indices)[i-2]; 
      i2 = (*m_Indices)[i-1];
    }
    i3 = (*m_Indices)[i];
    if (i2 > i1)
      order = true;
    else
      order = false;
    // compute cross product in right order
    if(order)
      triangle_normal = normalize(cross(m_Vertices->at(i2).pos-m_Vertices->at(i1).pos,m_Vertices->at(i3).pos-m_Vertices->at(i1).pos));
    else
      triangle_normal = normalize(cross(m_Vertices->at(i2).pos-m_Vertices->at(i3).pos,m_Vertices->at(i1).pos-m_Vertices->at(i3).pos));
    m_Vertices->at(i1).normal += triangle_normal;
    m_Vertices->at(i2).normal += triangle_normal;
    m_Vertices->at(i3).normal += triangle_normal;
    i1 = i2;
    i2 = i3;
  }
  for(unsigned int i = 0; i<m_Vertices->size();i++){
    if ((m_Vertices->at(i).pos).y < 1100.0f){
      m_Vertices->at(i).normal = normalize(m_Vertices->at(i).normal);
    }
  } 
}

void SkyMesh::compute_indices_sky(){
  int scale = CHUNK_SIZE*precision -(precision-1.0f);
  unsigned int u, v, w;
  float a;
  std::vector<Vertexun>::iterator iter = m_Vertices->begin();  
  for(unsigned m = 0; m < 2; m++){
    for(unsigned int k=0;k<CHUNK_PER_SIDE*CHUNK_PER_SIDE;k++){
      for(unsigned int i=0;i<=(CHUNK_SIZE-1) * precision -1;i++){
        for(unsigned int j=0;j<=(CHUNK_SIZE-1) * precision;j++){
          u = j+(i * scale) + scale*scale*k + m * (m_Vertices->size()/2);
          v = j+((i+1)*scale) + scale*scale*k + m * (m_Vertices->size()/2);
          if (((*(iter + u)).pos).y < 1100.0f){
            if ((m != 0)) {
              if (((i== 0) && (k/CHUNK_PER_SIDE != 0)) &&
                  ((j == 0) && (k%CHUNK_PER_SIDE != 0))){
                if ((((*(iter + u - (scale*scale*(CHUNK_PER_SIDE - 1) + 2*scale))).pos).y > 1100.0f) ||
                  (((*(iter + u - (scale*scale*(CHUNK_PER_SIDE - 1) + 2*scale - 1))).pos).y > 1100.0f) ||
                  (((*(iter + u + 1)).pos).y > 1100.0f) ||
                  (((*(iter + u + scale)).pos).y > 1100.0f) ||
                  (((*(iter + u - (scale*scale - (scale - 2)))).pos).y > 1100.0f) ||
                  (((*(iter + u - (scale*scale - (2*scale - 2)))).pos).y > 1100.0f)){
                  a = ((*(iter + u)).pos).y;
                  u -= (m_Vertices->size()/2);
                  ((*(iter+u)).pos).y += a;
                  ((*(iter+u)).pos).y /= 2;
                }
              }
              else if (((i== 0) && (k/CHUNK_PER_SIDE != 0)) && 
                      ((j == (CHUNK_SIZE-1)*precision) && (k%CHUNK_PER_SIDE!=CHUNK_PER_SIDE-1))){
                if ((((*(iter + u - (scale*scale*(CHUNK_PER_SIDE - 1) + 2*scale))).pos).y > 1100.0f) ||
                  (((*(iter + u - (scale*scale*(CHUNK_PER_SIDE - 2) + 3*scale - 2))).pos).y > 1100.0f) ||
                  (((*(iter + u - (scale*scale*(CHUNK_PER_SIDE - 2) + 2*scale - 2))).pos).y > 1100.0f) ||
                  (((*(iter + u - 1)).pos).y > 1100.0f) ||
                  (((*(iter + u + scale)).pos).y > 1100.0f) ||
                  (((*(iter + u + scale - 1)).pos).y > 1100.0f)){
                  a = ((*(iter + u)).pos).y;
                  u -= (m_Vertices->size()/2);
                  ((*(iter+u)).pos).y += a;
                  ((*(iter+u)).pos).y /= 2;
                }
              }
              else if ((i== 0) && (k/CHUNK_PER_SIDE != 0)){
                if ((((*(iter + u - 1)).pos).y > 1100.0f) ||
                  (((*(iter + u + 1)).pos).y > 1100.0f) ||
                  (((*(iter + u + scale)).pos).y > 1100.0f) ||
                  (((*(iter + u - (scale*scale*(CHUNK_PER_SIDE - 1) + 2*scale))).pos).y > 1100.0f) ||
                  (((*(iter + u - (scale*scale*(CHUNK_PER_SIDE - 1) + 2*scale - 1))).pos).y > 1100.0f) ||
                  (((*(iter + u + scale - 1)).pos).y > 1100.0f)){
                  a = ((*(iter + u)).pos).y;
                  u -= (m_Vertices->size()/2);
                  ((*(iter+u)).pos).y += a;
                  ((*(iter+u)).pos).y /= 2;
                }
              }
              else if ((j == 0) && (k%CHUNK_PER_SIDE != 0)){
                if((((*(iter + u + 1)).pos).y > 1100.0f) ||
                  (((*(iter + u + scale)).pos).y > 1100.0f) ||
                  (((*(iter + u - scale)).pos).y > 1100.0f) ||
                  (((*(iter + u - scale + 1)).pos).y > 1100.0f) ||
                  (((*(iter + u - (scale*scale - (scale - 2)))).pos).y > 1100.0f) ||
                  (((*(iter + u - (scale*scale - (2*scale - 2)))).pos).y > 1100.0f)){
                  a = ((*(iter + u)).pos).y;
                  u -= (m_Vertices->size()/2);
                  ((*(iter+u)).pos).y += a;
                  ((*(iter+u)).pos).y /= 2;
                }
              }
              else if ((j == (CHUNK_SIZE-1)*precision) && (k%CHUNK_PER_SIDE!=CHUNK_PER_SIDE-1)){
                if ((((*(iter + u - 1)).pos).y > 1100.0f) ||
                  (((*(iter + u + scale)).pos).y > 1100.0f) ||
                  (((*(iter + u - scale)).pos).y > 1100.0f) ||
                  (((*(iter + u + scale*scale - (scale-2))).pos).y > 1100.0f) ||
                  (((*(iter + u + scale*scale - (2*scale-2))).pos).y > 1100.0f) ||
                  (((*(iter + u + scale - 1)).pos).y > 1100.0f)){
                  a = ((*(iter + u)).pos).y;
                  u -= (m_Vertices->size()/2);
                  ((*(iter+u)).pos).y += a;
                  ((*(iter+u)).pos).y /= 2;
                }
              }
              else{
                if ((((*(iter + u - 1)).pos).y > 1100.0f) ||
                  (((*(iter + u + 1)).pos).y > 1100.0f) ||
                  (((*(iter + u + scale)).pos).y > 1100.0f) ||
                  (((*(iter + u - scale)).pos).y > 1100.0f) ||
                  (((*(iter + u - scale + 1)).pos).y > 1100.0f) ||
                  (((*(iter + u + scale - 1)).pos).y > 1100.0f)){
                  a = ((*(iter + u)).pos).y;
                  u -= (m_Vertices->size()/2);
                  ((*(iter+u)).pos).y += a;
                  ((*(iter+u)).pos).y /= 2;
                }
              }
            }
            w = u;
            m_Indices->push_back(u);
          }
          else{
            if (w != m_Vertices->size()){
              w = m_Vertices->size();
              m_Indices->push_back(w);
            }
          }
          if (((*(iter + v)).pos).y < 1100.0f){
            if (m != 0){
              if (((i== (CHUNK_SIZE-1)*precision-1) && (k/CHUNK_PER_SIDE != CHUNK_PER_SIDE-1)) && 
                  ((j == 0) && (k%CHUNK_PER_SIDE != 0))){
                if ((((*(iter + v + scale*scale*(CHUNK_PER_SIDE - 2) + 3*scale -2)).pos).y > 1100.0f) ||
                  (((*(iter + v + scale*scale*(CHUNK_PER_SIDE - 2) + 2*scale -2)).pos).y > 1100.0f) ||
                  (((*(iter + v + scale*scale*(CHUNK_PER_SIDE - 2) + 3*scale -1)).pos).y > 1100.0f) ||
                  (((*(iter + v + 1)).pos).y > 1100.0f) ||
                  (((*(iter + v - scale)).pos).y > 1100.0f) ||
                  (((*(iter + v - scale + 1)).pos).y > 1100.0f)){
                  a = ((*(iter + v)).pos).y;
                  v -= (m_Vertices->size()/2);
                  ((*(iter+v)).pos).y += a;
                  ((*(iter+v)).pos).y /= 2;
                }
              }
              else if (((i== (CHUNK_SIZE-1)*precision-1) && (k/CHUNK_PER_SIDE != CHUNK_PER_SIDE-1)) && 
                      ((j == (CHUNK_SIZE-1)*precision) && (k%CHUNK_PER_SIDE!=CHUNK_PER_SIDE-1))){
                if ((((*(iter + v - 1)).pos).y > 1100.0f) ||
                  (((*(iter + v - scale)).pos).y > 1100.0f) ||
                  (((*(iter + v + scale*scale - scale + 2)).pos).y > 1100.0f) ||
                  (((*(iter + v + scale*scale - 2*scale + 2)).pos).y > 1100.0f) ||
                  (((*(iter + v + scale*scale*(CHUNK_PER_SIDE - 1) + 2*scale)).pos).y > 1100.0f) ||
                  (((*(iter + v + scale*scale*(CHUNK_PER_SIDE - 1) + 2*scale - 1)).pos).y > 1100.0f)){
                  a = ((*(iter + v)).pos).y;
                  v -= (m_Vertices->size()/2);
                  ((*(iter+v)).pos).y += a;
                  ((*(iter+v)).pos).y /= 2;
                }
              }
              else if ((i== (CHUNK_SIZE - 1)*precision-1) &&
                      (k/CHUNK_PER_SIDE != CHUNK_PER_SIDE-1)){
                if ((((*(iter + v - 1)).pos).y > 1100.0f) ||
                  (((*(iter + v + 1)).pos).y > 1100.0f) ||
                  (((*(iter + v + scale*scale*(CHUNK_PER_SIDE-1) + 2*scale)).pos).y > 1100.0f) ||
                  (((*(iter + v - scale)).pos).y > 1100.0f) ||
                  (((*(iter + v - scale + 1)).pos).y > 1100.0f) ||
                  (((*(iter + v + scale*scale*(CHUNK_PER_SIDE-1) + 2*scale -1)).pos).y > 1100.0f)){
                  a = ((*(iter + v)).pos).y;
                  v -= (m_Vertices->size()/2);
                  ((*(iter+v)).pos).y += a;
                  ((*(iter+v)).pos).y /= 2;
                }
              }
              else if ((j == 0) && (k%CHUNK_PER_SIDE != 0)){
                if((((*(iter + v + 1)).pos).y > 1100.0f) ||
                  (((*(iter + v + scale)).pos).y > 1100.0f) ||
                  (((*(iter + v - scale)).pos).y > 1100.0f) ||
                  (((*(iter + v - scale + 1)).pos).y > 1100.0f) ||
                  (((*(iter + v - (scale*scale - (scale - 2)))).pos).y > 1100.0f) ||
                  (((*(iter + v - (scale*scale - (2*scale - 2)))).pos).y > 1100.0f)){
                  a = ((*(iter + v)).pos).y;
                  v -= (m_Vertices->size()/2);
                  ((*(iter+v)).pos).y += a;
                  ((*(iter+v)).pos).y /= 2;
                }
              }
              else if ((j == (CHUNK_SIZE-1)*precision) && (k%CHUNK_PER_SIDE!=CHUNK_PER_SIDE-1)){
                if ((((*(iter + v - 1)).pos).y > 1100.0f) ||
                  (((*(iter + v + scale)).pos).y > 1100.0f) ||
                  (((*(iter + v - scale)).pos).y > 1100.0f) ||
                  (((*(iter + v + scale*scale - (scale-2))).pos).y > 1100.0f) ||
                  (((*(iter + v + scale*scale - (2*scale-2))).pos).y > 1100.0f) ||
                  (((*(iter + v + scale - 1)).pos).y > 1100.0f)){
                  a = ((*(iter + v)).pos).y;
                  v -= (m_Vertices->size()/2);
                  ((*(iter+v)).pos).y += a;
                  ((*(iter+v)).pos).y /= 2;
                }
              }
              else{
                if ((((*(iter + v - 1)).pos).y > 1100.0f) ||
                  (((*(iter + v + 1)).pos).y > 1100.0f) ||
                  (((*(iter + v + scale)).pos).y > 1100.0f) ||
                  (((*(iter + v - scale)).pos).y > 1100.0f) ||
                  (((*(iter + v - scale + 1)).pos).y > 1100.0f) ||
                  (((*(iter + v + scale - 1)).pos).y > 1100.0f)){
                  a = ((*(iter + v)).pos).y;
                  v -= (m_Vertices->size()/2);
                  ((*(iter+v)).pos).y += a;
                  ((*(iter+v)).pos).y /= 2;
                }
              }
            }
            w = v;
            m_Indices->push_back(v);
          }
          else{
            if (w != m_Vertices->size()){
              w = m_Vertices->size();
              m_Indices->push_back(w);
            }
          }
        }
        if (w != m_Vertices->size()){
          w = m_Vertices->size();
          m_Indices->push_back(w);
        }
      }
    }
  }
}

void SkyMesh::initload_sky(glm::vec2 center){
  last_chunk = GetChunk(center);
  glm::i32vec2 firstChunk = (last_chunk-CHUNK_PER_SIDE/2);
  glm::i32vec2 firstPoint = (firstChunk*CHUNK_SIZE) - firstChunk;
  glm::i32vec2 offset = firstPoint;
  float a,b,c;
  for(int m = -1; m <= 1; m += 2){
    for(int k=0;k<CHUNK_PER_SIDE;){
      offset.x = firstPoint.x;
      for(int l=0;l<CHUNK_PER_SIDE;){
        buffer_map[k*CHUNK_PER_SIDE+l] = glm::i32vec2(firstChunk.x+l,firstChunk.y+k);
        for(int i=0;i<=(CHUNK_SIZE-1)*precision;i++){
          for(int j=0;j<=(CHUNK_SIZE-1)*precision;j++){
            a = (float)j/precision + offset.x;
            b = (float)i/precision + offset.y;
            if (m == -1) 
              c = (*m_VNoise).compute(a,b);
            else
              c = (*m_RNoise).compute(a,b);
            m_Vertices->push_back(Vertexun( glm::vec3(a, c, b), glm::vec2(), glm::vec3()));
          }
        }

        l++;
        offset.x = firstPoint.x + l * (CHUNK_SIZE-1);
      }
      k++;
      offset.y = firstPoint.y + k * (CHUNK_SIZE - 1);
    }
    offset.y = firstPoint.y;
  }
}

glm::i32vec2 SkyMesh::GetChunk(glm::vec2 coords){
  short signum_x = (coords.x > 0) - (coords.x < 0);
  short signum_y = (coords.y > 0) - (coords.y < 0);
  int p = ((-0.5f) * signum_x * signum_x + (0.5f) * signum_x);
  int r = ((-0.5f) * signum_y * signum_y + (0.5f) * signum_y);
  return  glm::i32vec2(p + coords.x/CHUNK_SIZE, r + coords.y/CHUNK_SIZE);
}

