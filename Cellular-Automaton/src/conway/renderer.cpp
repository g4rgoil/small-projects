#include "renderer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "../shaders.hpp"
#include "../util.hpp"


using namespace conway;

Renderer::Renderer(const glm::mat4 &coord_projection, const glm::mat4 &texture_projection)
{
    GLuint vertex_shader, fragment_shader;
    if (!LoadShader(&vertex_shader, GL_VERTEX_SHADER, shaders::ConwayVertexSource.c_str(),
                    shaders::ConwayVertexSource.size())) {
        exit(EXIT_FAILURE);
    }
    if (!LoadShader(&fragment_shader, GL_FRAGMENT_SHADER, shaders::ConwayFragmentSource.c_str(),
                    shaders::ConwayFragmentSource.size())) {
        exit(EXIT_FAILURE);
    }

    shader_program_ = glCreateProgram();
    glAttachShader(shader_program_, vertex_shader);
    glAttachShader(shader_program_, fragment_shader);
    if (!LinkProgram(shader_program_)) {
        exit(EXIT_FAILURE);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glUseProgram(shader_program_);
    glUniform1i(glGetUniformLocation(shader_program_, "grid"), 0);
    glUniformMatrix4fv(glGetUniformLocation(shader_program_, "projection"),
                       1, GL_FALSE, &coord_projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader_program_, "texProjection"),
                       1, GL_FALSE, &texture_projection[0][0]);
}

Renderer::~Renderer()
{
    glDeleteProgram(shader_program_);
}

void Renderer::draw(GLuint vertex_array, GLuint texture)
{
    glUseProgram(shader_program_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(vertex_array);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

GLuint Renderer::createTexture(GLsizei size_x, GLsizei size_y)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, size_x, size_y);

    float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return texture;
}