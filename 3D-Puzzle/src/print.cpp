//
// Created by pascal on 30.12.18.
//

#include <cstring>
#include <iostream>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "../include/print.hpp"


column_sizes::column_sizes() :
        id(0),
        name(0),
        mode(0),
        left_side(0),
        right_side(0)
{ }


void print_line(const LevelSide &side, u_long line, Mode mode)
{
    for (u_long x = 0; x < side.width; ++x) {
        if (mode == normal) {
            std::cout << (side.vector.at(line).at(x) ? "X" : "'") << " ";
        } else if (mode == picross) {
            int val = side.vector.at(line).at(x);
            std::cout << (char) (val ? '0' + val : '\'') << " ";
        }
    }
}


void print_line(const LevelSide &left, const LevelSide &right, u_long line, Mode mode)
{
    print_line(left, line, mode);
    std::cout << "   ";

    print_line(right, line, mode);
    std::cout << "   ";

    std::cout << "z=" << std::to_string(line) << std::endl;


}


void print(const Level &level)
{
    printf("left side: %lu x %lu, right side: %lu x %lu, mode: %s\n\n",
           level.left.width, level.left.height, level.right.width, level.right.height,
           level.mode == picross ? "picross" : "normal");

    print(level.left, level.right, level.mode);
}


void print(const LevelStackEntry &entry)
{
    printf("identifier: %d, ", entry.id);

    if (!entry.name.empty())
        printf("name: %s, ", entry.name.c_str());

    print(entry.level);
}

void print(const LevelSide &left, const LevelSide &right, Mode mode)
{
    for (int y = 1; y <= left.height; ++y) {
        print_line(left, right, left.height - y, mode);
    }
}


void get_column_sizes(const LevelStack &stack, column_sizes &columns)
{
    columns.id = 2;            // = len("id")
    columns.name = 4;          // = len("name")
    columns.mode = 7;          // = len("picross")
    columns.left_side = 9;     // = len("left side")
    columns.right_side = 10;   // = len("right side")

    for (const auto &entry : stack.vector) {
        columns.id = std::max(columns.id, (int) log10(entry.id) + 1);
        columns.name = std::max(columns.name, (int) entry.name.length());
        columns.left_side = std::max(columns.left_side,
                                     (int) (log10(entry.level.left.width) + log10(entry.level.left.height)) + 4);
        columns.right_side = std::max(columns.right_side,
                                      (int) (log10(entry.level.right.width) + log10(entry.level.right.height)) + 5);
    }
}


void print(const LevelStackEntry &entry, const column_sizes &columns, char *format_string)
{
    printf(format_string, std::to_string(entry.id).c_str(), entry.name.c_str(),
           entry.level.mode == picross ? "picross" : "normal",
           (std::to_string(entry.level.left.width) + " x " + std::to_string(entry.level.left.height)).c_str(),
           (std::to_string(entry.level.right.width) + " x " + std::to_string(entry.level.right.height)).c_str());
}


void print(const LevelStack &stack)
{
    // <id>  <name>  <mode>  <left_side>  <right_side>
    column_sizes columns;
    get_column_sizes(stack, columns);

    char fstring[100];
    snprintf(fstring, 100, "%%-%ds   %%-%ds   %%-%ds   %%-%ds   %%-%ds\n", columns.id, columns.name,
             columns.mode, columns.left_side, columns.right_side);

    printf(fstring, "id", "name", "mode", "left_side", "right_side");
    std::cout << std::endl;

    for (const auto &entry : stack.vector) {
        print(entry, columns, fstring);
    }
}


void center(const char *str, u_int width, char *buffer, u_int buffer_len)
{
    u_int len = std::max(width, (u_int) strlen(str));

    auto left = (u_int) std::floor((len - strlen(str)) / 2.f);
    auto right = (u_int) std::ceil((len - strlen(str)) / 2.f);

    assert(left + strlen(str) + right == len);

    snprintf(buffer, buffer_len, "%*c%s%*c", left, ' ', str, right, ' ');
}


void print_line(const Solution &solution, u_long y)
{
    u_long actual_y = solution.depth - y - 1;
    for (u_long z = 0; z < solution.height; ++z) {
        for (u_long x = 0; x < solution.width; ++x) {
            std::cout << (solution.at(x, actual_y, z) ? "X" : "'") << " ";
        }

        std::cout << "   ";
    }

    std::cout << "y=" << actual_y << std::endl;
}


void print(const Solution &solution)  // TODO limit line length?
{
    for (u_long y = 0; y < solution.depth; ++y) {
        print_line(solution, y);
    }

    u_int buffer_len = 2 * (u_int) solution.width;
    char buffer[buffer_len];

    for (u_long z = 0; z < solution.height; ++z) {
        center(("z=" + std::to_string(z)).c_str(), 2 * (u_int) solution.width - 1, buffer, buffer_len);
        std::cout << buffer << "    ";
    }

    std::cout << std::endl;
}
