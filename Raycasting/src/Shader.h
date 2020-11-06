#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>

class Shader {
public:
	/* file must contain vertex and fragment shader
	- first line of vertex shader must be: #shader vertex
	- first line of fragment shader must be: #shader fragment 
	The file will be closed automatically in this function */
	Shader(std::ifstream& fileStream);

	// manually input vertex and fragent shaders as std::strings
	Shader(const std::string& vertexShader, const std::string& fragmentShader);

	// stores the id of an already initialized shader program
	Shader(GLuint existingShader);

	void setUniform1f(const std::string& uniformName, float value);
	void setUniform2f(const std::string& uniformName, float* values);
	void setUniform3f(const std::string& uniformName, float* values);
	void setUniform4f(const std::string& uniformName, float* values);

	void setUniform1i(const std::string& uniformName, int value);

	void bind();
	void unbind();

	void freeMemory();

private:
	GLuint createShader(const std::string& vertexShader, const std::string& fragmentShader);
	GLuint compileShader(GLuint type, const std::string& source);

private:
	GLuint id;

};