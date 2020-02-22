#include "game.hpp"

#include <tuple>
#include <random>
#include <iostream>
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
        renderer_(), is_dirty_(true)
{
    std::default_random_engine random_engine;
    std::uniform_int_distribution<int> distribution(0, 1);

    generateMatrix(current_grid, [&](){
        return distribution(random_engine) ? State::alive : State::dead;});
    fillMatrix(alternate_grid, State::dead);

    texture_ = Renderer::createTexture(size_x_, size_y_);
    writeToTexture(texture_);
}

void ConwaysGame::draw(GLuint vertex_array)
{
    if (is_dirty_) {
        writeToTexture(texture_);
        is_dirty_ = false;
    }

    renderer_.draw(vertex_array, texture_);
}

void ConwaysGame::writeToTexture(GLuint texture)
{
    // Flips the image along the x axis (texture origin is bottom left)  ¯\_(ツ)_/¯
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