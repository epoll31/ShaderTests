#include "Window.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	Window* user = (Window*)glfwGetWindowUserPointer(window);
	user->UpdateBounds();
}

Window::Window(int width, int height, const char *title, GLfloat *vertices, size_t vertexCount, uint32_t *indices, size_t indexCount) {
	this->width = width;
	this->height = height;
	this->title = title;
	this->vertices = vertices;
	this->indices = indices;
	this->vertexCount = vertexCount;
	this->indexCount = indexCount;

	this->timer = 0;
	this->prev = 0;
	this->elapsed = 0;

	this->VBO = 0;
	this->VAO = 0;
	this->EBO = 0;

	this->window = NULL;

	this->wf_status = 1;

	this->sa_count = 0;
	this->sa_length = 0;
	this->sa_shaders = NULL;

	this->initialize();
}
Window::Window(int width, int height, GLfloat* vertices, size_t vertexCount, uint32_t* indices, size_t indexCount)
	: Window(width, height, "Window", vertices, vertexCount, indices, indexCount) { }

void Window::UpdateBounds() {
	glfwGetWindowSize(window, &this->width, &this->height);
}
int Window::GetWidth() {
	return this->width;
}
int Window::GetHeight() { 
	return this->height;
}

bool Window::Update()
{
	this->timer = glfwGetTime();
	this->elapsed = this->timer - this->prev;
	this->prev = this->timer;

	kp_process_inputs();

	if (glfwWindowShouldClose(this->window)) {
		return false;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < this->sa_count; i++) {
		this->sa_shaders[i].Use();
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glfwSwapBuffers(window);
	glfwPollEvents();

	return true;
}
void Window::Close()
{
	glfwTerminate();
}
double Window::GetTotalTime()
{
	return this->timer;
}
double Window::GetElapsedTime()
{
	return this->elapsed;
}

void Window::FlipWireframe()
{
	this->wf_status = !this->wf_status;
	if (this->wf_status) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void Window::AttachShader(Shader* shader) {
	if (this->sa_count == this->sa_length) {
		Shader* shaders = (Shader*)malloc(sizeof(Shader) * (++this->sa_length));
		for (int i = 0; i < this->sa_count; i++) {
			shaders[i] = this->sa_shaders[i];
		}
		free(this->sa_shaders);
		this->sa_shaders = shaders;
	}
	this->sa_shaders[this->sa_count++] = *shader;
}
bool Window::DetachShader(Shader* shader) {
	for (int i = 0; i < this->sa_count; i++) {
		if (this->sa_shaders + sizeof(Shader) * i == shader) {
			this->sa_shaders[i] = this->sa_shaders[this->sa_count - 1];
			free(this->sa_shaders + sizeof(Shader) * (this->sa_count - 1));
			this->sa_count--;
			return true;
		}
	}
	return false;
}

void Window::AttachKey(int key_code, KeyDownFunc key_down)
{
	this->kp_funcs[key_code] = key_down;
}
void Window::RemoveKey(int key_code) {
	this->kp_funcs[key_code] = NULL;
}

void Window::initialize() {
	//initialize glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//creating window
	this->window = glfwCreateWindow(this->width, this->height, this->title, NULL, NULL);
	if (this->window == NULL) {
		//window creation failed
		glfwTerminate();
		throw -1;
	}
	glfwSetWindowUserPointer(this->window, this);

	//set window to active
	glfwMakeContextCurrent(window);

	//initializing glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		//glad initialization failed
		glfwTerminate();
		throw -1;
	}

	//this handles resizing the window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//creates the viewport
	glViewport(0, 0, this->width, this->height);

	glGenBuffers(1, &this->VBO);
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertexCount * sizeof(GLfloat) * 6, this->vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indexCount * sizeof(GLfloat), this->indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); 
	glEnableVertexAttribArray(1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // default to fill
	
}
void Window::kp_process_inputs()
{
	for (int i = 0; i < UINT8_MAX; i++) {
		if (this->kp_funcs[i] == NULL) {
			continue;
		}
		bool prev = this->kp_prev_state[i];
		bool curr = glfwGetKey(this->window, i);
		
		if (curr && !prev) {
			this->kp_funcs[i](this);
		}

		this->kp_prev_state[i] = curr;
	}
}
