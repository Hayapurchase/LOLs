#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

void VAO::linkVBO(VBO VBO, GLuint layout)
{
	VBO.Bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	VBO.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(ID);
}
void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}