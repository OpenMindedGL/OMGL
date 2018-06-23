#include "IndexBuffer.h"
#include "Debug.h"
#include <GL/glew.h>

IndexBuffer::IndexBuffer() 
{
}

IndexBuffer::IndexBuffer(const void * d, unsigned int count)
	: m_Count(count), data(d)
{
	GLCall(glGenBuffers(1, &m_RenderID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RenderID));
}

void IndexBuffer::Bind() const 
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::Upload(unsigned int offset, unsigned int c){
        if(c == 0)
          c = m_Count;
        Bind();
	GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset*sizeof(unsigned int), c*sizeof(unsigned int), data));
}
