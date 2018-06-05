#ifndef Log_H
#define Log_H
#pragma once

#include <vector>
#include "Vertex.h"

#define PRINT 1

#define DEBUG 0
#define INFO 1
#define WARNING 2
#define ERROR 3


class Log 
{

private : 

public: 
	//Log();
        static void PrintVertices(std::vector<Vertexun>& v);
        static void PrintIndices(std::vector<unsigned int>& i);

};
#endif

