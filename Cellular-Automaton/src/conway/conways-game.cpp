#include "conways-game.hpp"

#include <tuple>
#include <random>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <boost/numeric/ublas/io.hpp>
#include <glad/glad.h>

#include "../util.hpp"
#include "../shaders.hpp"


using namespace conway;

template<class T>
void fillMatrix(ublas::matrix<T> &matrix, const T value)
{
    for (auto itr = matrix.begin1(); itr != matrix.end1(); ++itr) {
        std::fill(itr.begin(), itr.end(), value);
    }
}

template<class T, class Generator>
void generateMatrix(ublas::matrix<T> &matrix, Generator gen)
{
    for (auto itr = matrix.begin1(); itr != matrix.end1(); ++itr) {
        std::generate(itr.begin(), itr.end(), gen);
    }
}

ConwaysGame::ConwaysGame(uint size_x, uint size_y) :
        size_x_(size_x), size_y_(size_y),
        current_grid(size_x, size_y),
        alternate_grid(size_x, size_y),
        is_dirty_(true)
{
    std::default_random_engine random_engine;
    std::uniform_int_distribution<int> distribution(0, 1);

    generateMatrix(current_grid, [&](){
        return distribution(random_engine) ? State::alive : State::dead;});
    fillMatrix(alternate_grid, State::dead);

    GLuint vertex_shader, fragment_shader;
    if (!LoadShader(&vertex_shader, GL_VERTEX_SHADER, shaders::ConwayVertexSource.c_str())) {
        exit(EXIT_FAILURE);
    }
    if (!LoadShader(&fragment_shader, GL_FRAGMENT_SHADER, shaders::ConwayFragmentSource.c_str())) {
        exit(EXIT_FAILURE);
    }

    glUseProgram(fragment_shader);
    glUniform1i(glGetUniformLocation(fragment_shader, "grid"), 0);

    shader_program_ = glCreateProgram();
    glAttachShader(shader_program_, vertex_shader);
    glAttachShader(shader_program_, fragment_shader);
    if (!LinkProgram(shader_program_)) {
        exit(EXIT_FAILURE);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    // Flips the image along the x axis (texture origin is bottom left)  ¯\_(ツ)_/¯
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, size_x, size_y_);
    writeToTexture(texture_);

    float borderColor[] = { 0.0, 0.0f, 0.0f, 1.0f };
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

sf::Vector2u ConwaysGame::getSize() const
{
    return {size_x_, size_y_};
}

void ConwaysGame::draw(GLuint vertex_array)
{
    if (is_dirty_) {
        writeToTexture(texture_);
        is_dirty_ = false;
    }

    glUseProgram(shader_program_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glBindVertexArray(vertex_array);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void ConwaysGame::writeToTexture(GLuint texture)
{
    glTextureSubImage2D(texture, 0, 0, 0, size_x_, size_y_,
        GL_RED, GL_UNSIGNED_BYTE, current_grid.data().begin());
}

void ConwaysGame::nextGeneration()
{
    for (auto itr = alternate_grid.begin1(); itr != alternate_grid.end1(); ++itr) {
        for (auto cell = itr.begin(); cell != itr.end(); ++cell) {
            int x = cell.index1(), y = cell.index2();
            *cell = nextState(current_grid(x, y), countNeighbours(x, y));
        }
    }

    is_dirty_ = true;
    std::swap(current_grid, alternate_grid);
}

State ConwaysGame::nextState(State state, int neighbours)
{
    if (neighbours == 3) {
        return State::alive;
    } else if (neighbours == 2 && state == State::alive) {
        return State::alive;
    } else {
        return State::dead;
    }
}

int ConwaysGame::countNeighbours(int x, int y) const
{
    int neighbours = 0;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0)
                continue;

            const int xdx = x + dx, ydy = y + dy;
            if (0 <= xdx && xdx < size_x_ && 0 <= ydy && ydy < size_y_) {
                neighbours += current_grid(xdx, ydy);
            }
        }
    }

    return neighbours;
}