#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	unsigned int ID;
	Shader(const char* vertexpath,const char* fragmentpath);
	void Activate();
	void Delete();

private:
	// Checks if the different Shaders have compiled properly
	void compileErrors(unsigned int shader, const char* type);
};

#endif // !SHADER_H
