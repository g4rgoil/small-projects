#ifndef CONWAY_RENDERER_HPP_INCLUDED
#define CONWAY_RENDERER_HPP_INCLUDED

#include <glad/glad.h>


namespace conway {

class Renderer
{
  public:
    Renderer();
    void draw(GLuint vertex_array, GLuint texture);
    static GLuint createTexture(GLsizei size_x, GLsizei size_y);


  private:
    GLuint shader_program_;
};

}

#endif  // CONWAY_RENDERER_HPP_INCLUDED