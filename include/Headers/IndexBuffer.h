#ifndef IndexBuffer_H
#define IndexBuffer_H
#pragma once

class IndexBuffer {
private:
	unsigned int m_RenderID;
	unsigned int m_Count;
        const void* data;

public:

	IndexBuffer();
	IndexBuffer(const void* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count;  }
	inline void SetCount(unsigned int c) { m_Count = c;  }
        void Upload(unsigned int offset = 0, unsigned int c = 0);
};
#endif
