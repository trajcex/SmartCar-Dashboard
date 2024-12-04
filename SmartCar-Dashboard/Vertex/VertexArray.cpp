#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &ID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &ID);
}

void VertexArray::bind() const
{
	glBindVertexArray(ID);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbl)
{
	vb.bind();
	const auto& elements = vbl.getElements();
	for (unsigned int i = 0;  i < elements.size() ; i++)
	{
		const VertexBufferElement element = elements[i];
		glVertexAttribPointer(element.index, element.size, element.type, element.normalized, vbl.getStride(), (void*)(element.offset*sizeof(float)));
		glEnableVertexAttribArray(i);
	}
	vb.unbind();
}

