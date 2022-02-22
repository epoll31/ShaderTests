#include "Window.h"

/*void framebuffer_size_callback_func(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}*/


Window::Window(int width, int height, GLfloat *vertices, uint32_t *indices) {
	this->width = width;
	this->height = height;
	this->title = "Window";
	this->vertices = vertices;
	this->indices = indices;

	this->timer = 0;
	this->prev = 0;
	this->elapsed = 0;

	this->VBO = 0;
	this->VAO = 0;
	this->EBO = 0;

	this->window = NULL;
}
Window::Window(int width, int height, const char *title, GLfloat* vertices, uint32_t* indices) {
	this->width = width;
	this->height = height;
	this->title = title;
	this->vertices = vertices;
	this->indices = indices;

	this->timer = 0;
	this->prev = 0;
	this->elapsed = 0;

	this->VBO = 0;
	this->VAO = 0;
	this->EBO = 0;

	this->window = NULL;
}

int Window::GetWidth()
{
	glfwGetWindowSize(this->window, &width, &height);
	return this->width;
}

int Window::GetHeight()
{ 
	glfwGetWindowSize(this->window, &width, &height);
	return this->height;
}

bool Window::Update()
{
	this->timer = glfwGetTime();
	this->elapsed = this->timer - this->prev;
	this->prev = this->timer;
	return !glfwWindowShouldClose(this->window);
}

double Window::GetTotalTime()
{
	return this->timer;
}

double Window::GetElapsedTime()
{
	return this->elapsed;
}

void Window::SetWireframe(uint8_t wf_status)
{
	if ((wf_status >> 0) & 1 && !((this->wf_status >> 0) & 1)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // default to fill
	}
	if ((wf_status >> 0) & 1 && !((this->wf_status >> 0) & 1)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // default to fill
	}
	this->wf_status = wf_status;
}

void Window::AttachKey(int key_code, void(*keyDown)(int))
{

}

void Window::initialize() {
	//initialize glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//creating window
	window = glfwCreateWindow(this->width, this->height, this->title, NULL, NULL);
	if (window == NULL) {
		//window creation failed
		glfwTerminate();
		throw -1;
	}

	//set window to active
	glfwMakeContextCurrent(window);

	//initializing glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		//glad initialization failed
		glfwTerminate();
		throw -1;
	}

	//this handles resizing the window
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_func);

	//creates the viewport
	glViewport(0, 0, this->width, this->height);

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // default to fill
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe
}

void Window::processInput()
{
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(this->window, true);
	}
}
