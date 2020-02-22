#ifndef ABSTRACT_GAME_HPP_INCLUDED
#define ABSTRACT_GAME_HPP_INCLUDED

#include <glad/glad.h>

class AbstractGame
{
public:
    virtual void draw(GLuint vertex_array) = 0;
    virtual void nextGeneration() = 0;
};

#endif  // ABSTRACT_GAME_HPP_INCLUDED