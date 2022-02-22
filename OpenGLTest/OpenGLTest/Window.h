#pragma once

#ifndef _glfw3_h_
#include <GLFW/glfw3.h>
#endif

#ifndef __glad_h_
#include <glad/glad.h>
#endif

#ifndef gif_h
#include "gif.h"
#endif

#define WF_WIRE 2
#define WF_FILL 1

class Window
{
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

	GLfloat *vertices = NULL;
	uint32_t *indices = NULL;
	GLFWwindow* window = NULL;

	uint8_t wf_status = 1;
	//key code count
	uint8_t *kc_codes = NULL;
	void (*keyFuncs)(int) = NULL;
	
	void initialize();
	void processInput();

public:
	Window(int width, int height, GLfloat* vertices, uint32_t* indices);
	Window(int width, int height, const char* title, GLfloat* vertices, uint32_t* indices);

	int GetWidth();
	int GetHeight();

	bool Update();
	double GetTotalTime();
	double GetElapsedTime();

	void SetWireframe(uint8_t wfStatus);
	void AttachKey(int key_code, void (*keyDown)(int));
};

