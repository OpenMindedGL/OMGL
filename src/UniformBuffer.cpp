#include "UniformBuffer.h"
#include "Renderer.h"

UniformBuffer::UniformBuffer() 
{
}

UniformBuffer::UniformBuffer(const void* data, unsigned int size, unsigned int binding ) : m_Data(data), m_Size(size), m_BindingID(binding)
{
	GLCall(glGenBuffers(1, &m_RenderID));
        Bind();
	GLCall(glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

void UniformBuffer::Upload(unsigned int offset, unsigned int s){
        /*if(s == 0)
          s = size;
        Bind();
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, s, data));*/
}


UniformBuffer::~UniformBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RenderID));
}

void UniformBuffer::Bind(unsigned int binding) const 
{
	GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RenderID));
}

void UniformBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER,0));
}
