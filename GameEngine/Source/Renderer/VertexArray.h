#pragma once

#include "Vertex.h"
#include "Texture.h"
#include <vector>

class VertexArray
{
public:
	
	VertexArray( const Vertex* vertices, const unsigned int vertexCount);

	//For indexed drawing
	VertexArray( const Vertex* vertices, const unsigned int vertexCount, const unsigned int* indices, const unsigned int indexCount );

	VertexArray( const std::vector<Vertex>& vertices);

	VertexArray( const std::vector<Vertex>& vertices,  const std::vector<unsigned int>& indices);


	~VertexArray();
	
	void bind() const;
	void unbind() const;

	unsigned int getVertexCount() const { return count; }
	unsigned int getIndexCount() const{ return indexCount; }
	bool isIndexed() const { return EBO != 0; } // Check if we use indices
private:
	
	//Vertex array object, Vertex buffer object, Elememnt buffer object
	unsigned int VAO, VBO, EBO = 0;
	unsigned int count; //Number of vertices;
	unsigned int indexCount;
	void setupAttributes();
};