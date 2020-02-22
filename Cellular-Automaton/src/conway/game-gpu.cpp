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

void CreateTexture(GLuint *texture, uint size_x, uint size_y)
{
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, size_x, size_y);

    float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

ConwaysGameGpu::ConwaysGameGpu(uint size_x, uint size_y) :
        size_x_(size_x), size_y_(size_y), renderer_()
{
    CreateTexture(&current_texture_, size_x_, size_y_);
    CreateTexture(&alternate_texture_, size_x_, size_y_);

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
        exit(EXIT_FAILURE);
    }

    glDeleteShader(compute_shader);
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

    glDispatchCompute(size_x_ / 16, size_y_ / 16, 1);
    // glMemoryBarrier(GL_ALL_BARRIER_BITS);
    std::swap(current_texture_, alternate_texture_);
}