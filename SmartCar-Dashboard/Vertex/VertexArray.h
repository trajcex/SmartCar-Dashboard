#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H
#include <GL/glew.h>
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"

class VertexArray
{
private:
	unsigned int ID;
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbl);

	void bind() const;
	void unbind() const;
};

#endif
