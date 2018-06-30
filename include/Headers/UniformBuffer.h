#ifndef UniformBuffer_H
#define UniformBuffer_H
#pragma once
#define DEFAULT_BINDING_INDEX      0

class UniformBuffer {
private : 
	unsigned int m_RenderID;
        unsigned int m_BindingID;
        const void* m_Data;
        unsigned int m_Size;

public : 
	UniformBuffer();
	UniformBuffer(const void* data, unsigned int size, unsigned int binding = DEFAULT_BINDING_INDEX);
	~UniformBuffer();

        void Upload(unsigned int offset = 0, unsigned int s = 0);
	void Bind() const { Bind(m_BindingID); }
	void Bind( unsigned int binding_id ) const;
	void Unbind() const;

        // getters
        unsigned int GetBindIndex(){ return m_BindingID; }
};
#endif
