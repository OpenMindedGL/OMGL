#include "Log.h"
#include "Vertex.h"
#include <vector>
#include <stdio.h>

bool Log::isOn = false;

void Log::PrintVertices(std::vector<Vertexun>& v){

  Vertexun& a = v.at(0);
  for(unsigned int j=0; j < v.size(); j++){
    a = v.at(j);
    printf("%d: (%f,%f,%f)\n",j,a.pos.x,a.pos.y,a.pos.z);
  }

}

void Log::PrintIndices(std::vector<unsigned int>& i){

  for(unsigned int j=0; j < i.size(); j++){
    printf("%d: %d\n",j,i.at(j));
  }

}
