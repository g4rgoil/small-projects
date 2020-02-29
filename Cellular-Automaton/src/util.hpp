#ifndef UTIL_HPP_INCLUDED
#define UTIL_HPP_INCLUDED

#include <glad/glad.h>
#include <string>


GLuint CreateProgram(GLenum type, const std::string &shader_src);
GLuint CreateProgram(const std::string &vertex_src, const std::string &fragment_src);
GLuint CreateShader(GLenum type, const char *source, int length);
void LinkProgram(GLuint program);

#endif  // UTIL_HPP_INCLUDED