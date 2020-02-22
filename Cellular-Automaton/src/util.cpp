#include "util.hpp"

#include <glad/glad.h>
#include <vector>
#include <string>
#include <iostream>

bool LoadShader(GLuint *shader, GLenum type, const char *source, int length)
{
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, &length);
    glCompileShader(*shader);

    GLint success = 0;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(*shader, maxLength, &maxLength, &errorLog[0]);

        // std::string log(errorLog.begin(), errorLog.end());
        std::cout << std::string(errorLog.begin(), errorLog.end()) << std::endl;
        glDeleteShader(*shader);

        return false;
    }

    return true;
}

bool LinkProgram(GLuint program)
{
    glLinkProgram(program);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);

        std::cout << std::string(errorLog.begin(), errorLog.end()) << std::endl;
        glDeleteProgram(program);

        return false;
    }

    return true;
}