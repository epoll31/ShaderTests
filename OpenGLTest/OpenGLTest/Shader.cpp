#include "Shader.h"

const char* Shader::ReadFile(const char *fileName) {
    std::ifstream inFile;
    inFile.open(fileName);
    if (!inFile) {
        std::cout << "invalid file name" << std::endl;
        exit(-1);
    }
    std::string full;
    std::string line;
    while (getline(inFile, line)) {
        full += line + '\n';
    }
    inFile.close();

    full += '\0';

    char* output = new char[full.length() + 1];
    std::copy(full.begin(), full.end(), output);
    return output;
}

unsigned int Shader::CompileShader(const char* shaderSource, GLenum shaderType) {
    unsigned int shader;
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
        exit(-1);
    }
    return shader;
}

Shader::Shader(const char* vertexFileName, const char* fragmentFileName)
{
    const char* vertexShaderSource = ReadFile(vertexFileName);
    const char* fragmentShaderSource = ReadFile(fragmentFileName);
    unsigned int vertexShader = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
    unsigned int fragmentShader = CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    this->program = glCreateProgram();
    glAttachShader(this->GetNumber(), vertexShader);
    glAttachShader(this->GetNumber(), fragmentShader);
    glLinkProgram(this->GetNumber());
    int success;
    glGetProgramiv(this->GetNumber(), GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(this->GetNumber(), 512, NULL, infoLog);
        std::cout << "linking shaders failed:\n" << infoLog << std::endl;
        exit(-1);
    }

    glUseProgram(this->GetNumber());
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Use()
{
    glUseProgram(this->GetNumber());
}

unsigned int Shader::GetNumber()
{
    return this->program;
}

void Shader::SetUniform(const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(this->GetNumber(), name), (int)value);
}

void Shader::SetUniform(const char* name, int value)
{
    glUniform1i(glGetUniformLocation(this->GetNumber(), name), value);
}

void Shader::SetUniform(const char* name, float x)
{
    glUniform1f(glGetUniformLocation(this->GetNumber(), name), x);
}

void Shader::SetUniform(const char* name, float x, float y)
{
    glUniform2f(glGetUniformLocation(this->GetNumber(), name), x, y);
}

void Shader::SetUniform(const char* name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(this->GetNumber(), name), x, y, z);
}

void Shader::SetUniform(const char* name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(this->GetNumber(), name), x, y, z, w);
}

void Shader::SetUniform(const char* name, const GLfloat *points, int numPoints)
{
    glUniform3fv(glGetUniformLocation(this->GetNumber(), name), numPoints, points);
}
