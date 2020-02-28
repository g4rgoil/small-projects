#ifndef CONWAY_RENDERER_HPP_INCLUDED
#define CONWAY_RENDERER_HPP_INCLUDED

#include <glad/glad.h>
#include <glm/glm.hpp>


namespace conway {

class Renderer
{
  public:
    Renderer(const glm::mat4 &coord_projection, const glm::mat4 &texture_projection);
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = default;
    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = default;
    ~Renderer();

    void draw(GLuint vertex_array, GLuint texture);
    static GLuint createTexture(GLsizei size_x, GLsizei size_y);

  private:
    GLuint shader_program_;
};

}

#endif  // CONWAY_RENDERER_HPP_INCLUDED