//
// Created by pascal on 30.12.18.
//

#ifndef INC_3D_PUZZLE_PRINT_HPP
#define INC_3D_PUZZLE_PRINT_HPP


#include "puzzle.hpp"


struct column_sizes
{
    int id;
    int name;
    int mode;
    int left_side;
    int right_side;

    column_sizes();
};


void print(const Level &level);

void print(const LevelStackEntry &entry);

void print(const LevelSide &left, const LevelSide &right, Mode mode);

void print(const LevelStack &stack);

void print(const Solution &solution);


#endif //INC_3D_PUZZLE_PRINT_HPP
