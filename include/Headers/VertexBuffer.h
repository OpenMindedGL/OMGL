#ifndef VertexBuffer_H
#define VertexBuffer_H
#pragma once

class VertexBuffer {
private : 
	unsigned int m_RenderID;
        const void* data;
        unsigned int size;

public : 
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

        void Upload(unsigned int offset = 0, unsigned int s = 0);
	void Bind() const;
	void Unbind() const;
};
#endif
