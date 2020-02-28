#ifndef CONWAY_GAME_GPU_HPP_INCLUDED
#define CONWAY_GAME_GPU_HPP_INCLUDED

#include <cmath>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../abstract_game.hpp"
#include "renderer.hpp"


namespace conway {

class ConwaysGameGpu: public AbstractGame
{
  public:
    ConwaysGameGpu(GLsizei size_x, GLsizei size_y, const glm::mat4 &projection);
    ConwaysGameGpu(const ConwaysGameGpu&) = delete;
    ConwaysGameGpu(ConwaysGameGpu&&) = default;
    ConwaysGameGpu& operator=(const ConwaysGameGpu&) = delete;
    ConwaysGameGpu& operator=(ConwaysGameGpu&&) = default;
    ~ConwaysGameGpu();

    glm::uvec2 getSize() const;
    void draw(GLuint vertex_array);
    void nextGeneration();

  private:
    const uint size_x_;
    const uint size_y_;

    Renderer renderer_;
    GLuint current_texture_;
    GLuint alternate_texture_;
    GLuint compute_program_;
};

}

#endif  // CONWAY_GAME_GPU_HPP_INCLUDED