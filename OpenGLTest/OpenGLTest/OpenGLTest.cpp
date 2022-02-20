#include <iostream>
#include <string>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Shader.h"
#include "OpenGLTest.h"

#include "gif.h"

#define CreateGif 0

bool isWireframe = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int lastWPress = GLFW_RELEASE;
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    int wPress = glfwGetKey(window, GLFW_KEY_W);
    if (wPress == GLFW_PRESS && lastWPress == GLFW_RELEASE) {
        isWireframe = !isWireframe;
        if (isWireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
    lastWPress = wPress;
}

void populateCircles(const int circles, GLfloat *points, float *infos, float width, float height) {
    for (int i = 0; i < circles; i++) {
        int pxi = i * 3;    //index of x
        int pyi = pxi + 1;  //index of y
        int pri = pxi + 2;  //index of r

        int di = i * 2;     //index of direction
        int si = di + 1;    //index of speed

        points[pri] = (std::rand() % 15) + 10;
        points[pxi] = width / 2;// (std::rand() % (int)(width - 4 * points[pri])) + 2 * points[pri];
        points[pyi] = height / 2;// (std::rand() % (int)(height - 4 * points[pri])) + 2 * points[pri];
        infos[di] = (std::rand() % 1000) / 1000.0F * acos(0) * 2;
        infos[si] = (std::rand() % 50) + 10;
    }
}

GLfloat* createColorArray(const int count, uint32_t* input) {
    GLfloat *output = (GLfloat*)malloc(sizeof(GLfloat) * count * 3);
    for (int i = 0; i < count; i++) {
        output[i * 3 + 0] = ((input[i] & 0xff0000) >> 16) / 255.0f;
        output[i * 3 + 1] = ((input[i] & 0x00ff00) >>  8) / 255.0f;
        output[i * 3 + 2] = ((input[i] & 0x0000ff) >>  0) / 255.0f;
    }
    return output;
}

int main()
{
    srand(NULL);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    float vertices[] = {
         //positions            //colors
         1.0f,  1.0f, 0.0f,//     1.0f, 0.0f, 0.0f, //top right
         1.0f, -1.0f, 0.0f,//     0.0f, 1.0f, 0.0f, //bottom right
        -1.0f, -1.0f, 0.0f,//     0.0f, 0.0f, 1.0f, //bottom left
        -1.0f,  1.0f, 0.0f//,     1.0f, 1.0f, 1.0f,  //top left
    };

    unsigned int indices[] = {
        0, 1, 3, //first triangle
        1, 2, 3  //second triangle
    };
    /*float vertices[] = {
        //positions          
        -0.50f,  0.75f, 0.00f,      // 0: top left
         0.50f,  0.75f, 0.00f,      // 1: top right
        -0.25f, -0.25f, 0.00f,      // 2: mid left
         0.25f, -0.25f, 0.00f,      // 3: mid right
        -0.50f, -0.75f, 0.00f,      // 4: bottom left
         0.50f, -0.75f, 0.00f       // 5: bottom right
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3,
        2, 3, 4,
        3, 4, 5
    };
    */
    /*GLfloat points[] = { // length should always be circles * 3
        250, 250, 50,
        400, 100, 25
    };
    float infos[]{ // length should always be circles * 2
        1, 100,
        3, 200
    };
    */



    GLFWwindow* window = glfwCreateWindow(500, 500, "OpenGLTest", NULL, NULL);
    if (window == NULL) {
        std::cout << "window creation failed" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    
    const int circles = 10;
    GLfloat points[circles * 3];
    float infos[circles * 2];
    populateCircles(circles, points, infos, width, height);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "initialize glad failed" << std::endl;
        return -1;
    }
    const int numColors = 5;
    const GLfloat *colors = createColorArray(numColors, new uint32_t[]{ 0x2D728F, 0x3B8EA5, 0xF5EE9E, 0xF49E4C, 0xAB3428 });

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glViewport(0, 0, 500, 500);

    Shader *shader = new Shader("basicVertexShader.glsl", "basicFragmentShader.glsl");

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//positions
    glEnableVertexAttribArray(0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));//colors
    //glEnableVertexAttribArray(1);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//default to fill

    float prev = 0;
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

    while (!glfwWindowShouldClose(window))
    {
        glfwGetWindowSize(window, &width, &height);

        float time = glfwGetTime();
        float elapsed = time - prev;

        processInput(window);

        for (int i = 0; i < circles; i++) {
            int pxi = i * 3;    //index of x
            int pyi = pxi + 1;  //index of y
            int pri = pxi + 2;  //index of r

            int di = i * 2;     //index of direction
            int si = di + 1;    //index of speed

            points[pxi] += infos[si] * (float)cos(infos[di]) * elapsed;
            points[pyi] += infos[si] * (float)sin(infos[di]) * elapsed;

            if (points[pxi] - points[pri] < 0 || points[pxi] + points[pri] > width) {
                infos[di] = 2 * acos(0) - infos[di];
                points[pxi] += infos[si] * (float)cos(infos[di]) * elapsed;
            }
            if (points[pyi] - points[pri] < 0 || points[pyi] + points[pri] > height) {
                infos[di] *= -1;
                points[pyi] += infos[si] * (float)sin(infos[di]) * elapsed;
            }
        }

        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        shader->SetUniform("pointArray", points, circles);
        shader->SetUniform("size", 0.01f);
        shader->SetUniform("colors", colors, numColors);
        shader->Use();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

#if CreateGif
        uint8_t* buffer = new uint8_t[width * height * 4];
        glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8, buffer);
        for (int i = 0; i < width * height; i++) {
            uint8_t temp = buffer[i * 4 + 0];          
            buffer[i * 4 + 0] = buffer[i * 4 + 1];
            buffer[i * 4 + 1] = buffer[i * 4 + 2];
            buffer[i * 4 + 2] = buffer[i * 4 + 3];
            buffer[i * 4 + 3] = temp;
        }
        GifWriteFrame(&gw, buffer, width, height, delay, 8, true);

        imgNum++;
        if (imgNum == frames) {
            GifEnd(&gw);
            return 0;
        }
#endif

        glfwSwapBuffers(window);
        glfwPollEvents();

        prev = time;
    }

    glfwTerminate();
    return 0;
}

