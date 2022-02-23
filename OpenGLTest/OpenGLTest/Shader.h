#pragma once
#ifndef _SHADER_H_
#define _SHADER_H_

#ifndef _IOSTREAM_
#include <iostream>
#endif
#ifndef _FSTREAM_
#include <fstream>
#endif
#ifndef _STRING_
#include <string>
#endif

#ifndef __glad_h_
#include <glad/glad.h>
#endif

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

#endif // _SHADER_H_