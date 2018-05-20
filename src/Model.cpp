#include "Vertex.h"
#include "Model.h"

template <>
void Model<Vertex>::Push()
{
  m_Layout.Push<float>(3);
}

template <>
void Model<Vertexu>::Push()
{
  m_Layout.Push<float>(3);
  m_Layout.Push<float>(2);
}

template <>
void Model<Vertexun>::Push()
{
  m_Layout.Push<float>(3);
  m_Layout.Push<float>(2);
  m_Layout.Push<float>(3);
}
