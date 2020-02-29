#include "util.hpp"

#include <vector>

#include "exceptions.hpp"


GLuint CreateProgram(GLenum type, const std::string &shader_src)
{
    GLuint shader = CreateShader(type, shader_src.c_str(), shader_src.size());
    GLuint program = glCreateProgram();

    glAttachShader(program, shader);
    LinkProgram(program);
    glDeleteShader(shader);

    return program;
}

GLuint CreateProgram(const std::string &vertex_src, const std::string &fragment_src)
{
    GLuint vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_src.c_str(), vertex_src.size());
    GLuint fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_src.c_str(), fragment_src.size());
    GLuint program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    LinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

GLuint CreateShader(GLenum type, const char *source, int length)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, &length);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

        glDeleteShader(shader);
        throw OpenGlError(std::string(errorLog.begin(), errorLog.end()));
    }

    return shader;
}

void LinkProgram(GLuint program)
{
    glLinkProgram(program);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);

        glDeleteProgram(program);
        throw OpenGlError(std::string(errorLog.begin(), errorLog.end()));
    }
}