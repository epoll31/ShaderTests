#include "Window.h"

int main() {
	GLfloat vertices[(3 + 3) * 4] = {
		-0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 0.0f
	};
	uint32_t triangles[3 * 2] = {
		0, 1, 2,
		1, 2, 3
	};

	Window *window = new Window(500, 500, vertices, (3 + 3) * 4, triangles, 6);
	Shader *shader = new Shader("vertexShader.glsl", "fragmentShader.glsl");
	window->AttachShader(shader);
	
	while (window->Update()) {

	}

	window->Close();
}