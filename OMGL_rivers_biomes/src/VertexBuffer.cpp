#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer() 
{
}

VertexBuffer::VertexBuffer(const void * data, unsigned int s)
{
	GLCall(glGenBuffers(1, &m_RenderID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, s, data, GL_STATIC_DRAW));
}

void VertexBuffer::Upload(){
        Bind();
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}


VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RenderID));
}

void VertexBuffer::Bind() const 
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER,0));
}