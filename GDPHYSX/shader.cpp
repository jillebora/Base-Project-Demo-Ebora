#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>


std::string Shader::loadFile(const char* path) const
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Failed to open shader file: " << path << std::endl;
        return std::string();
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void Shader::checkCompileErrors(GLuint object, const std::string& type) const
{
    GLint success = 0;
    char info[1024];

    if (type == "PROGRAM")
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, sizeof(info), nullptr, info);
            std::cerr << "Shader program link error:\n" << info << std::endl;
        }
    }
    else
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, sizeof(info), nullptr, info);
            std::cerr << type << " shader compile error:\n" << info << std::endl;
        }
    }
}


Shader::Shader()
    : ID(0)
{
}


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertS = loadFile(vertexPath);
    std::string fragS = loadFile(fragmentPath);

    const char* v = vertS.c_str();
    const char* f = fragS.c_str();

    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, nullptr);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &f, nullptr);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    // Link program
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // Stages are baked in — free them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    if (ID != 0)
        glDeleteProgram(ID);
}

// ---- Public methods --------------------------------------------------------

void Shader::use() const
{
    if (ID != 0)
        glUseProgram(ID);
}

void Shader::destroy() { if (ID != 0) { glDeleteProgram(ID); ID = 0; } }

void Shader::setBool(const std::string& name, bool value) const
{
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniform1i(loc, (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniform1i(loc, value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniform1f(loc, value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& v) const
{
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniform3fv(loc, 1, &v[0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& m) const
{
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}