//
// Created by pascal on 30.12.18.
//


#include <algorithm>
#include <climits>
#include <numeric>
#include <assert.h>
#include "../include/solve.hpp"


void init_solution(const Level &level, Solution &solution)
{
    solution.width = level.right.width;
    solution.depth = level.left.width;
    solution.height = level.left.height;

    solution.vector.resize((u_long) solution.height);
    for (auto &v : solution.vector) {
        v.resize((u_long) solution.width * solution.depth);
    }
}


bool solve_min(const Level &level, Solution &solution)
{
    init_solution(level, solution);

    bool result = false;
    for (u_long z = 0; z < level.right.height; ++z) {
        result |= solve_min(level, solution, z);
    }

    return result;
}


bool solve_min(const Level &level, Solution &solution, u_long z_level)
{
    const level_line &x_vector = level.right.vector.at(z_level);
    const level_line &y_vector = level.left.vector.at(z_level);
    solution_line &solution_vector = solution.vector.at(z_level);

    if ((std::find(x_vector.begin(), x_vector.end(), true) != x_vector.end()) != (
            std::find(y_vector.begin(), y_vector.end(), true) != y_vector.end())) {
        return false;
    }

    int x_max_used = -1, y_max_used = -1;
    int x_max = 0, y_max = 0;

    for (int x = 0; x < level.right.width; ++x)
        x_max = x_vector[x] ? x : x_max;

    for (int y = 0; y < level.left.width; ++y)
        y_max = y_vector[y] ? y : y_max;

    for (int y = 0, i = 0; y < level.left.width; ++y) {
        for (int x = 0; x < level.right.width; ++x, ++i) {
            if (x_vector[x] && y_vector[y]) {
                if ((x > x_max_used && y > y_max_used) ||
                    (x == x_max && y > y_max_used) ||
                    (x > x_max_used && y == y_max)) {

                    x_max_used = x, y_max_used = y;
                    solution_vector[i] = true;
                }
            }
        }
    }

    return true;
}


bool solve_max(const Level &level, Solution &solution)
{
    init_solution(level, solution);

    bool result = false;
    for (u_long z = 0; z < level.right.height; ++z) {
        result |= solve_max(level, solution, z);
    }

    return result;
}


bool solve_max(const Level &level, Solution &solution, u_long z_level)
{
    const level_line &x_vector = level.right.vector.at(z_level);
    const level_line &y_vector = level.left.vector.at(z_level);
    solution_line &solution_vector = solution.vector.at(z_level);

    if (x_vector.empty() || y_vector.empty()) {
        return false;
    }

    if ((find(x_vector.begin(), x_vector.end(), true) != x_vector.end()) != (
            find(y_vector.begin(), y_vector.end(), true) != y_vector.end())) {
        return false;
    }

    for (int y = 0, i = 0; y < y_vector.size(); ++y) {
        for (int x = 0; x < x_vector.size(); ++x, ++i) {
            solution_vector[i] = x_vector[x] && y_vector[y];
        }
    }

    return true;
}


bool solve_picross(const Level &level, Solution &solution)
{
    init_solution(level, solution);

    bool result = false;
    for (u_long z = 0; z < level.right.height; ++z) {
        result |= solve_picross(level, solution, z);
    }

    return result;
}


int index_of_largest(
        const level_line &vec,
        const std::vector<bool> &exclude)
{
    assert(vec.size() == exclude.size());

    int index = -1, largest = INT_MIN;

    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i] > largest && !exclude[i]) {
            largest = vec[i];
            index = i;
        }
    }

    return index;
}


bool solve_picross(const Level &level, Solution &solution, u_long z_level)
{
    const level_line &x_vector = level.right.vector.at(z_level);
    const level_line &y_vector = level.left.vector.at(z_level);
    solution_line &solution_vector = solution.vector.at(z_level);

    if (x_vector.empty() || y_vector.empty()) {
        return false;
    }

    if (accumulate(x_vector.begin(), x_vector.end(), 0) != accumulate(y_vector.begin(), y_vector.end(), 0)) {
        return false;
    }

    level_line x_copy(x_vector), y_copy(y_vector);

    const std::vector<bool> x_exclude(x_vector.size(), false);

    for (int squares_left = accumulate(x_vector.begin(), x_vector.end(), 0); squares_left > 0;) {
        int largest_x = index_of_largest(x_copy, x_exclude);

        for (int i = x_vector[largest_x]; i > 0; --i, --squares_left) {
            std::vector<bool> y_exclude(y_vector.size(), false);
            int largest_y = 0;

            do {
                largest_y = index_of_largest(y_copy, y_exclude);
                y_exclude[largest_y] = true;
            } while (largest_y != -1 && solution_vector[largest_y * x_vector.size() + largest_x]);

            solution_vector[largest_y * x_vector.size() + largest_x] = true;
            x_copy[largest_x]--;
            y_copy[largest_y]--;
        }
    }

    return true;
}

