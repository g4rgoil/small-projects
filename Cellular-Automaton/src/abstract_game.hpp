#ifndef ABSTRACT_GAME_HPP_INCLUDED
#define ABSTRACT_GAME_HPP_INCLUDED

#include <glad/glad.h>
#include <glm/glm.hpp>

class AbstractGame
{
  public:
    virtual ~AbstractGame() {};

    virtual glm::uvec2 getSize() const = 0;
    virtual void draw(GLuint vertex_array) = 0;
    virtual void nextGeneration() = 0;
};

#endif  // ABSTRACT_GAME_HPP_INCLUDED