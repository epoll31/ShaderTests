

#ifndef _IOSTREAM_
#include <iostream>
#endif
#ifndef _STRING_
#include <string>
#endif
  
#include "Window.h"

#ifndef gif_h
#include "gif.h"
#endif

#define CreateGif 0

void flip_wireframe(Window* window) {
    window->FlipWireframe();
}

void close(Window* window) {
    window->Close();
}

void populateCircles(const int circles, GLfloat *points, GLfloat *infos, int width, int height) {
    for (int i = 0; i < circles; i++) {
        int pxi = i * 3;    //index of x
        int pyi = pxi + 1;  //index of y
        int pri = pxi + 2;  //index of r

        int di = i * 2;     //index of direction
        int si = di + 1;    //index of speed

        points[pri] = (GLfloat)((std::rand() % 15) + 10);
        points[pxi] = (GLfloat)(width / 2);// (std::rand() % (int)(width - 4 * points[pri])) + 2 * points[pri];
        points[pyi] = (GLfloat)(height / 2);// (std::rand() % (int)(height - 4 * points[pri])) + 2 * points[pri];
        infos[di] = (GLfloat)((std::rand() % 1000) / 1000.0F * acos(0) * 2);
        infos[si] = (GLfloat)((std::rand() % 50) + 10);
    }
}

GLfloat* createColorArray(const int count, uint32_t* input) {
    GLfloat *output = (GLfloat*)malloc(sizeof(GLfloat) * count * 3);
    if (output == NULL) {
        return NULL;
    }
    for (int i = 0; i < count; i++) {
        output[i * 3 + 0] = (GLfloat)(((input[i] & 0xff0000) >> 16) / 255.0f);
        output[i * 3 + 1] = (GLfloat)(((input[i] & 0x00ff00) >>  8) / 255.0f);
        output[i * 3 + 2] = (GLfloat)(((input[i] & 0x0000ff) >>  0) / 255.0f);
    }
    return output;
}

int main()
{
    srand(NULL);

    float vertices[] = {
         //positions            //colors
         0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f, //top right
         0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f, //bottom right
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f, //bottom left
        -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 1.0f  //top left
    };

    unsigned int indices[] = {
        0, 1, 3, //first triangle
        1, 2, 3  //second triangle
    };
    
    Window* window = new Window(500, 500, "OpenGLTest", vertices, 4, indices, 6);
    Shader* shader = new Shader("basicVertexShader.glsl", "basicFragmentShader.glsl");
    Shader* shader2 = new Shader("vertexShader.glsl", "fragmentShader.glsl");
    window->AttachShader(shader);
    window->AttachShader(shader2);
    window->AttachKey(GLFW_KEY_W, &flip_wireframe);
    window->AttachKey(GLFW_KEY_ESCAPE, &close);

    const int numColors = 5;
    const GLfloat* colors = createColorArray(numColors, new uint32_t[]{ 0x2D728F, 0x3B8EA5, 0xF5EE9E, 0xF49E4C, 0xAB3428 });
    
    const int circles = 10;
    GLfloat points[circles * 3];
    GLfloat infos[circles * 2];
    populateCircles(circles, points, infos, window->GetWidth(), window->GetHeight());

    while (window->Update()) {
        for (int i = 0; i < circles; i++) {
            int pxi = i * 3;    //index of x
            int pyi = pxi + 1;  //index of y
            int pri = pxi + 2;  //index of r

            int di = i * 2;     //index of direction
            int si = di + 1;    //index of speed

            points[pxi] += infos[si] * (float)cos(infos[di]) * (float)window->GetElapsedTime();
            points[pyi] += infos[si] * (float)sin(infos[di]) * (float)window->GetElapsedTime();

            if (points[pxi] - points[pri] < 0 || points[pxi] + points[pri] > window->GetWidth()) {
                infos[di] = 2 * acos(0) - infos[di];
                points[pxi] += infos[si] * (float)cos(infos[di]) * (float)window->GetElapsedTime();
            }
            if (points[pyi] - points[pri] < 0 || points[pyi] + points[pri] > window->GetHeight()) {
                infos[di] *= -1;
                points[pyi] += infos[si] * (float)sin(infos[di]) * (float)window->GetElapsedTime();
            }
        }

        shader->SetUniform("pointArray", points, circles);
        shader->SetUniform("size", 0.01f);
        shader->SetUniform("colors", colors, numColors);
    }
    window->Close();

   
    double prev = 0;
#if CreateGif
    int imgNum = 0;
    std::string path = "C:\\Users\\epoll\\Documents\\Visual Studio 2022\\Projects\\OpenGLTest\\Images\\";
    std::string name = "test";;
    std::string extension = ".gif";
    std::string full = path + name + extension;
    GifWriter gw;
    const int delay = 1;
    const int frames = 250;
    GifBegin(&gw, full.c_str(), width, height, delay, 8, true);
#endif

    return 0;
}

