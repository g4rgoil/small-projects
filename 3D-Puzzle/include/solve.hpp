//
// Created by pascal on 30.12.18.
//

#ifndef INC_3D_PUZZLE_SOLVE_HPP
#define INC_3D_PUZZLE_SOLVE_HPP


#include "puzzle.hpp"


bool solve_min(const Level &level, Solution &solution);

bool solve_min(const Level &level, Solution &solution, u_long z_level);

bool solve_max(const Level &level, Solution &solution);

bool solve_max(const Level &level, Solution &solution, u_long z);

bool solve_picross(const Level &level, Solution &solution);

bool solve_picross(const Level &level, Solution &solution, u_long z);


#endif //INC_3D_PUZZLE_SOLVE_HPP
