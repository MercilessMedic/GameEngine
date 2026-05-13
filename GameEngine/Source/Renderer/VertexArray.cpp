#include "VertexArray.h"

#include <glad/glad.h>
#include <cstddef>

VertexArray::VertexArray( const Vertex* vertices, const unsigned int vertexCount ):
	count( vertexCount ), indexCount(0), EBO(0)
{
	//Generate Vertex array object and buffer
	glGenVertexArrays( 1, &VAO );
	glGenBuffers( 1, &VBO );

	glBindVertexArray( VAO );
	
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	
	glBufferData( GL_ARRAY_BUFFER, count * sizeof(Vertex), vertices, GL_STATIC_DRAW );

	setupAttributes();
	
	//Unbdind VAO  and VBO
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindVertexArray( 0 );

}

//Constructor for Indexed Drawing
VertexArray::VertexArray(const Vertex* vertices, const unsigned int vertexCount, const unsigned int* indices, const unsigned int indexCount)
	: count(vertexCount), indexCount(indexCount)
{
	//Generate Vertex array object and buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//Setup VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	//Setup EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	setupAttributes();

	glBindVertexArray(0);
}

VertexArray::VertexArray( const std::vector<Vertex>& vertices ):
	VertexArray( vertices.data(), vertices.size())
{}

VertexArray::VertexArray( const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices ):
	VertexArray( vertices.data(), vertices.size(), indices.data(), indices.size())
{}

VertexArray::~VertexArray()
{
	printf("VertexArray destructor called!\n");
	glDeleteVertexArrays( 1, &VAO );
	glDeleteBuffers( 1, &VBO );
	if( EBO != 0 ){ glDeleteBuffers(1, &EBO); }
}

void VertexArray::bind() const
{
	glBindVertexArray( VAO );
}

void VertexArray::unbind() const
{
	glBindVertexArray( 0 );
}

void VertexArray::setupAttributes()
{
	//Attribute 0: Position
	glEnableVertexAttribArray( (GLuint)VertAttrib::POSITION );
	glVertexAttribPointer(( GLuint)VertAttrib::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position) );
	//Attribute 1: Texture coordinates
	glEnableVertexAttribArray( (GLuint)VertAttrib::TEXCOORDS );
	glVertexAttribPointer( (GLuint)VertAttrib::TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords) );
	//Attribute 2: Surface Normal
	glEnableVertexAttribArray( (GLuint)VertAttrib::NORMAL );
	glVertexAttribPointer( (GLuint)VertAttrib::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal) );
	//Attribute 3: Tangent
	glEnableVertexAttribArray( (GLuint)VertAttrib::TANGENT );
	glVertexAttribPointer( (GLuint)VertAttrib::TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent) );
	//Attribute 4: Bitangent
	glEnableVertexAttribArray( (GLuint)VertAttrib::BITANGENT );
	glVertexAttribPointer( (GLuint)VertAttrib::BITANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	// Attribute 5: Bone IDs 
	glEnableVertexAttribArray( (GLuint)VertAttrib::BONE_IDS );
	glVertexAttribIPointer( (GLuint)VertAttrib::BONE_IDS, MAX_BONE_INFLUENCE, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, BoneIDs) );
	// Attribute 6: Bone Weights
	glEnableVertexAttribArray( (GLuint)VertAttrib::BONE_WEIGHTS );
	glVertexAttribPointer( (GLuint)VertAttrib::BONE_WEIGHTS, MAX_BONE_INFLUENCE, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, BoneWeights) );

}