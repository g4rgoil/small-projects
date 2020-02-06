#include "universe.hpp"

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

Universe::Universe(size_t _size_x, size_t _size_y) :
    size_x(_size_x), size_y(_size_y), cell_grid(_size_x, _size_y), neighbour_grid(_size_x, _size_y)
{
    std::default_random_engine random_engine;
    std::uniform_int_distribution<int> distribution(0, 1);

    generateMatrix(cell_grid, [&](){return distribution(random_engine) ? State::alive : State::dead;});
    fillMatrix(neighbour_grid, 0);
}

size_t Universe::sizeX() const
{
    return size_x;
}

size_t Universe::sizeY() const
{
    return size_y;
}

void Universe::makeImage(sf::Image &image) const
{
    for (auto itr = cell_grid.begin1(); itr != cell_grid.end1(); ++itr) {
        for (auto cell = itr.begin(); cell != itr.end(); ++cell) {
            image.setPixel(cell.index1(), cell.index2(), (*cell == State::alive) ? sf::Color::Black : sf::Color::White);
        }
    }
}

void Universe::nextGeneration()
{
    for (auto itr = cell_grid.begin1(); itr != cell_grid.end1(); ++itr) {
        for (auto cell = itr.begin(); cell != itr.end(); ++cell) {
            if (*cell == State::alive) {
                increaseNeighbourhood(cell.index1(), cell.index2());
            }
        }
    }

    auto [cell_itr, neighbour_itr] = std::tuple{cell_grid.begin1(), neighbour_grid.begin1()};
    for (; cell_itr != cell_grid.end1(); ++cell_itr, ++neighbour_itr) {
        auto [cell, neighbours] = std::tuple{cell_itr.begin(), neighbour_itr.begin()};
        for (; cell != cell_itr.end(); ++cell, ++neighbours) {
            *cell = nextCellState(*cell, *neighbours);
            *neighbours = 0;
        }
    }
}

void Universe::increaseNeighbourhood(int x, int y)
{
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0)
                continue;

            int xdx = x + dx, ydy = y + dy;
            if (0 <= xdx && xdx < size_x && 0 <= ydy && ydy < size_y) {
                neighbour_grid(xdx, ydy) += 1;
            }
        }
    }
}

State Universe::nextCellState(State state, int neighbours)
{
    if (neighbours == 3) {
        return State::alive;
    } else if (state == State::alive && neighbours == 2) {
        return State::alive;
    } else {
        return State::dead;
    }
}