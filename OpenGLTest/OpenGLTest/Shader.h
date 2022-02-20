#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <glad/glad.h>

class Shader
{
public:
	Shader(const char* vertexFileName, const char* fragmentFileName);
	void Use();
	unsigned int GetNumber();
	void SetUniform(const char *name, bool value);
	void SetUniform(const char *name, int value);
	void SetUniform(const char *name, float value);
	void SetUniform(const char *name, float x, float y);
	void SetUniform(const char *name, float x, float y, float z);
	void SetUniform(const char *name, float x, float y, float z, float w);
	void SetUniform(const char* name, const GLfloat* points, int numPoints);

private:
	unsigned int program;
	const char* ReadFile(const char *fileName);
	unsigned int CompileShader(const char* shaderSource, GLenum shaderType);
};

