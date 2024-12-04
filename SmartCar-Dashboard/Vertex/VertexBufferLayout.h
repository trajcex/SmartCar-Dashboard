#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H
#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

struct VertexBufferElement
{
	unsigned int index;   
	int size;             
	GLenum type;          
	GLboolean normalized;    
	unsigned int offset;
};
class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride;
public:
	VertexBufferLayout();

	void push(VertexBufferElement vbe);
	std::vector<VertexBufferElement> getElements() const;
	unsigned int getStride() const;
};

#endif
