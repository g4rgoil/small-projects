#include "conways-game.hpp"

#include <tuple>
#include <random>
#include <iostream>
#include <boost/numeric/ublas/io.hpp>

template<typename T>
void fillMatrix(ublas::matrix<T> &matrix, const T value)
{
    for (auto itr = matrix.begin1(); itr != matrix.end1(); ++itr) {
        std::fill(itr.begin(), itr.end(), value);
    }
}

template<typename T, class Generator>
void generateMatrix(ublas::matrix<T> &matrix, Generator gen)
{
    for (auto itr = matrix.begin1(); itr != matrix.end1(); ++itr) {
        std::generate(itr.begin(), itr.end(), gen);
    }
}

ConwaysGame::ConwaysGame(size_t _size_x, size_t _size_y) :
    size_x(_size_x), size_y(_size_y), current_grid(_size_x, _size_y), alternate_grid(_size_x, _size_y)
{
    std::default_random_engine random_engine;
    std::uniform_int_distribution<int> distribution(0, 1);

    generateMatrix(current_grid, [&](){return distribution(random_engine) ? State::alive : State::dead;});
    fillMatrix(alternate_grid, State::dead);
}

sf::Vector2<size_t> ConwaysGame::getSize() const
{
    return {size_x, size_y};
}

void ConwaysGame::makeImage(sf::Image &image) const
{
    for (auto itr = current_grid.begin1(); itr != current_grid.end1(); ++itr) {
        for (auto cell = itr.begin(); cell != itr.end(); ++cell) {
            image.setPixel(cell.index1(), cell.index2(), (*cell == State::alive) ? sf::Color::Black : sf::Color::White);
        }
    }
}

void ConwaysGame::nextGeneration()
{
    for (auto itr = alternate_grid.begin1(); itr != alternate_grid.end1(); ++itr) {
        for (auto cell = itr.begin(); cell != itr.end(); ++cell) {
            int x = cell.index1(), y = cell.index2();
            *cell = nextState(current_grid(x, y), countNeighbours(x, y));
        }
    }

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
            if (0 <= xdx && xdx < size_x && 0 <= ydy && ydy < size_y) {
                neighbours += current_grid(xdx, ydy);
            }
        }
    }

    return neighbours;
}