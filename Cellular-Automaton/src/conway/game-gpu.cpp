#include "game-gpu.hpp"

#include <random>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <glad/glad.h>

#include "../util.hpp"


using namespace conway;

ConwaysGameGpu::ConwaysGameGpu(GLsizei size_x, GLsizei size_y) :
        size_x_(size_x), size_y_(size_y),
        current_texture_(Renderer::createTexture(size_x, size_y)),
        alternate_texture_(Renderer::createTexture(size_x, size_y))
{
    std::default_random_engine random_engine;
    std::uniform_int_distribution<char> distribution(0, 1);
    std::vector<char> random_data(size_x_ * size_y_);
    std::generate(random_data.begin(), random_data.end(),
                  [&](){return distribution(random_engine);});
    glTextureSubImage2D(current_texture_, 0, 0, 0, size_x_, size_y_,
                        GL_RED, GL_UNSIGNED_BYTE, random_data.data());

    GLuint compute_shader;
    std::ifstream stream("/home/pascal/repositories/small-projects/Cellular-Automaton/shader/update.glsl");
    std::string source((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

    if (!LoadShader(&compute_shader, GL_COMPUTE_SHADER, source.c_str(), source.size())) {
        exit(EXIT_FAILURE);
    }

    compute_program_ = glCreateProgram();
    glAttachShader(compute_program_, compute_shader);
    if (!LinkProgram(compute_program_)) {
        glDeleteShader(compute_shader);
        exit(EXIT_FAILURE);
    }

}

ConwaysGameGpu::~ConwaysGameGpu()
{
    glDeleteTextures(1, &current_texture_);
    glDeleteTextures(1, &alternate_texture_);
    glDeleteProgram(compute_program_);
}

glm::uvec2 ConwaysGameGpu::getSize() const
{
    return {size_x_, size_y_};
}

void ConwaysGameGpu::draw(GLuint vertex_array)
{
    renderer_.draw(vertex_array, current_texture_);
}

void ConwaysGameGpu::nextGeneration()
{
    glUseProgram(compute_program_);
    glBindImageTexture(0, current_texture_, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R8I);
    glBindImageTexture(1, alternate_texture_, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R8I);

    glDispatchCompute(size_x_ / 16, size_y_ / 16, 1);  // TODO calculate properly
    glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
    std::swap(current_texture_, alternate_texture_);
}