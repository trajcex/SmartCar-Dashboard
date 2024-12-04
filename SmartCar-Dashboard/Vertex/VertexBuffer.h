#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H
#include <GL/glew.h>   

class VertexBuffer
{
public:
	VertexBuffer(const float* data, unsigned long size);
	~VertexBuffer();
	
	void bind() const;
	void unbind() const;
	unsigned int getID() const { return ID; };
private:
	unsigned int ID;
};

#endif
