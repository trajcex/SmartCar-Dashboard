#include "VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout() : stride(0){}

void VertexBufferLayout::push(VertexBufferElement vbe)
{
	elements.push_back(vbe);
	stride += (vbe.size * sizeof(float));
}

std::vector<VertexBufferElement> VertexBufferLayout::getElements() const
{
	return elements;
}

unsigned int VertexBufferLayout::getStride() const
{
	return stride;
}
