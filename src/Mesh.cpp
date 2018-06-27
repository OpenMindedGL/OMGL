#include "Vertex.h"
#include "Mesh.h"

template <>
void Mesh<Vertex>::Push()
{
  m_Layout.Push<float>(3);
}

template <>
void Mesh<Vertexu>::Push()
{
  m_Layout.Push<float>(3);
  m_Layout.Push<float>(2);
}

template <>
void Mesh<Vertexun>::Push()
{
  m_Layout.Push<float>(3);
  m_Layout.Push<float>(2);
  m_Layout.Push<float>(3);
}
/*
template <>
void Mesh<Vertexfun>::Push()
{
  m_Layout.Push<float>(1);
  m_Layout.Push<float>(2);
  m_Layout.Push<float>(3);
}

template <>
void Mesh<Vertexfn>::Push()
{
  m_Layout.Push<float>(1);
  m_Layout.Push<float>(3);
}
*/
