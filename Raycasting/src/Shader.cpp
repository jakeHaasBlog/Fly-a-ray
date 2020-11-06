#include "Shader.h"

#include <iostream>
#include <sstream>

Shader::Shader(std::ifstream & fileStream) {

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[3];
	ShaderType type = ShaderType::NONE;
	while (getline(fileStream, line)) {
		if (line.find("#shader") != std::string::npos) {

			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
			std::cout << line << std::endl;
		}
	}

	fileStream.close();

	id = createShader(ss[0].str(), ss[1].str());
}

Shader::Shader(const std::string & vertexShader, const std::string & fragmentShader) {
	id = createShader(vertexShader, fragmentShader);
}

Shader::Shader(GLuint existingShader) {
	id = existingShader;
}

void Shader::setUniform1f(const std::string & uniformName, float value) {
	bind();
	setUniform1f(uniformName, value);
	unbind();
}

void Shader::setUniform2f(const std::string & uniformName, float * values) {
	bind();
	setUniform2f(uniformName, values);
	unbind();
}

void Shader::setUniform3f(const std::string & uniformName, float * values) {
	bind();
	setUniform3f(uniformName, values);
	unbind();
}

void Shader::setUniform4f(const std::string & uniformName, float * values) {
	bind();
	setUniform4f(uniformName, values);
	unbind();
}

void Shader::setUniform1i(const std::string & uniformName, int value) {
	bind();
	setUniform1i(uniformName, value);
	unbind();
}

void Shader::bind() {
	glUseProgram(id);
}

void Shader::unbind() {
	glUseProgram(0);
}

void Shader::freeMemory() {
	glDeleteProgram(id);
	unbind();
}

GLuint Shader::createShader(const std::string & vertexShader, const std::string & fragmentShader)
{
	GLuint program = glCreateProgram();
	GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

GLuint Shader::compileShader(GLuint type, const std::string& source) {

	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile "
			<< (type == GL_VERTEX_SHADER ? "vertex shader" : GL_GEOMETRY_SHADER ? "geometry shader" : "fragment shader")
			<< std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		delete[] message;
		return 0;
	}

	return id;
}
