#pragma once

#ifndef _glfw3_h_
#include <GLFW/glfw3.h>
#endif

#ifndef __glad_h_
#include <glad/glad.h>
#endif

#ifndef gif_h
//#include "gif.h"
#endif

#ifndef _SHADER_H_
#include "Shader.h"
#endif

class Window
{
	typedef void (*KeyDownFuncArray[UINT8_MAX])(Window*);
	typedef void (*KeyDownFunc)(Window*);
	typedef bool KeyDownStateArray[UINT8_MAX];

private:
	int width = 0;
	int height = 0;

	const char* title;

	double timer = 0;
	double prev = 0;
	double elapsed = 0;

	uint32_t VBO = 0; // Vertex Buffer Object
	uint32_t VAO = 0; // Vertex Array Object
	uint32_t EBO = 0; // Element Buffer Object

	GLfloat* vertices = NULL;	uint32_t vertexCount = 0;
	uint32_t *indices = NULL;	uint32_t indexCount = 0;
	GLFWwindow* window = NULL;

	bool wf_status = 0;

	KeyDownFuncArray kp_funcs;//array of size 255
	KeyDownStateArray kp_prev_state;//array of bits size 255
	
	Shader *shader;

	void initialize();
	void kp_process_inputs();

public:
	Window(int width, int height, const char* title, GLfloat* vertices, uint32_t vertexCount, uint32_t* indices, uint32_t indexCount);
	Window(int width, int height, GLfloat* vertices, uint32_t vertexCount, uint32_t* indices, uint32_t indexCount);

	void UpdateBounds();
	int GetWidth();
	int GetHeight();

	bool Update();
	void Close();

	double GetTotalTime();
	double GetElapsedTime();

	void FlipWireframe();
	void AttachShader(Shader* shader);

	/// <summary>
	/// Will override current key_down func if already attached
	/// </summary>
	/// <param name="key_code"> Key Code range: [0-255] </param>
	/// <param name="keyDown">Function Pointer to void(Window*)</param>
	void AttachKey(int key_code, KeyDownFunc key_down);
	void RemoveKey(int key_code);
};
