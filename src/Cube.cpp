#include "Cube.h"

Cube::Cube() {}

Cube::Cube(unsigned int renderType, std::string shaderPath)
	: Model(renderType, &m_InitVertexUN, &m_Indices, shaderPath)
{}