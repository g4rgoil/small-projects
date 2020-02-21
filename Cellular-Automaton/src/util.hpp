#ifndef UTIL_HPP_INCLUDED
#define UTIL_HPP_INCLUDED

bool LoadShader(unsigned int *shader, unsigned int type, const char *source);
bool LinkProgram(unsigned int program);

#endif  // UTIL_HPP_INCLUDED