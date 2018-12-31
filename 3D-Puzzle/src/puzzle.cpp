//
// Created by pascal on 27.12.18.
//


// TODO Add support for picross constraint > 9


#include <iostream>
#include <regex>
#include <climits>
#include <stdio.h>
#include <boost/algorithm/string.hpp>
#include "../include/help.hpp"
#include "../include/puzzle.hpp"
#include "../include/print.hpp"
#include "../include/solve.hpp"


const help_map_type help_map = {  // NOLINT(cert-err58-cpp)
        {QUIT,          HELP_QUIT},
        {SET,           HELP_SET},
        {GET,           HELP_GET},
        {PUSH,          HELP_PUSH},
        {POP,           HELP_POP},
        {UNSET,         HELP_UNSET},
        {DELETE,        HELP_DELETE},
        {CLEAR,         HELP_CLEAR},
        {PRINT,         HELP_PRINT},
        {LIST,          HELP_LIST},
        {SOLVE,         HELP_SOLVE},
        {SOLVE_MIN,     HELP_SOLVE_MIN},
        {SOLVE_MAX,     HELP_SOLVE_MAX},
        {SOLVE_PICROSS, HELP_SOLVE_PICROSS},
};


/* ================================================================================================================== */


LevelSide::LevelSide() :
        width(0),
        height(0),
        vector(level_vector())
{ }


LevelSide::LevelSide(u_long width, u_long height, level_vector &vector) :
        width(width),
        height(height),
        vector(vector)
{ }


LevelSide::LevelSide(const LevelSide &other) = default;


int LevelSide::at(u_long x, u_long y) const
{
    return vector.at(y).at(x);
}


void LevelSide::assign(u_long x, u_long y, int value)
{
    vector[y][x] = value;
}


Level::Level(const Level &other) :
        mode(other.mode),
        left(LevelSide(other.left)),
        right(LevelSide(other.right))
{ }


Level::Level(Mode _mode, const LevelSide &_left, const LevelSide &_right) :
        mode(_mode),
        left(_left),
        right(_right)
{ }


Solution::Solution() :
        width(0),
        depth(0),
        height(0),
        vector(solution_vector())
{ }


bool Solution::at(u_long x, u_long y, u_long z) const
{
    return vector.at(z).at(y * width + x);
}


void Solution::assign(u_long x, u_long y, u_long z, bool value)
{
    vector[z][y * width + x] = value;
}


LevelStackEntry::LevelStackEntry(int _id, string &_name, const Level &_level) :
        id(_id),
        name(_name),
        level(_level)
{ }


LevelStack::LevelStack() noexcept :
    next_id(0),
    vector(std::vector<LevelStackEntry>())
{ }


Level *current_level = nullptr;
LevelStack level_stack;


/* ================================================================================================================== */


int parse_stack_index(const std::vector<string> &params)
{
    if (std::regex_match(params[0], std::regex("[0-9]+"))) {
        int level_id;
        std::istringstream convert(params[0]);

        if (!(convert >> level_id)) {
            std::cout << "The specified numerical identifier is not a valid number." << std::endl;
            return -1;
        }

        for (int i = 0; i < level_stack.vector.size(); ++i) {
            if (level_stack.vector.at((u_long) i).id == level_id) {
                return i;
            }
        }

        std::cout << "The specified numerical identifier doesn't point to an existing entry of the stack." << std::endl;
        return -1;

    } else {
        for (int i = 0; i < level_stack.vector.size(); ++i) {
            if (boost::iequals(params[0], level_stack.vector.at((u_long) i).name)) {
                return i;
            }
        }

        std::cout << "The specified name doesn't point to an existing entry of the stack." << std::endl;
        return -1;
    }
}


bool is_valid_level_line(const string &line, const Mode mode)
{
    if (mode == normal) {
        return std::regex_match(line, std::regex("[\\s\\-xX#]+"));
    } else if (mode == picross) {
        return std::regex_match(line, std::regex("[\\s\\-0-9]+"));
    }

    return false;
}


void read_line_array(std::vector<string> &lines, u_long *width, const Mode mode)
{
    *width = ULONG_MAX;
    string line;
    lines.clear();

    while (std::getline(std::cin, line)) {
        if (!line.empty()) {
            if (*width == ULONG_MAX) {
                *width = (int) line.length();
            } else if (*width != (int) line.length()) {
                std::cout << std::endl << "Every line must have the same length. "
                          << "Please enter the line again." << std::endl;
                continue;
            } else if (!is_valid_level_line(line, mode)) {
                std::cout << std::endl << "The line contains an invalid character. "
                          << "Please enter the line again." << std::endl;
                continue;
            }

            lines.push_back(line);
        } else {
            break;
        }
    }

    if (*width == ULONG_MAX) {
        std::cout << "The level side is smaller than the minimum of 1x1. Please enter the side again." << std::endl;
        read_line_array(lines, width, mode);
    }
}


int parse_char(char c, const Mode mode)
{
    // Assumes the character to be a valid symbol in the specified mode.
    if (mode == normal) {
        return (c == 'x' || c == 'X' || c == '#') ? 1 : 0;
    } else if (mode == picross) {
        return (c == ' ' || c == '-') ? 0 : c - '0';
    }

    return 0;
}


void parse_level_side(LevelSide &level_side, const Mode mode)
{
    std::vector<string> lines;
    read_line_array(lines, &level_side.width, mode);
    level_side.height = (int) lines.size();

    level_side.vector.clear();
    level_side.vector.resize((u_long) level_side.height);

    for (auto &v : level_side.vector) {
        v.resize(level_side.width);
    }


    for  (u_long y = 0; y < level_side.height; ++y) {
        for (u_long x = 0; x < level_side.width; ++x) {
            level_side.assign(x, level_side.height - y - 1, parse_char(lines[y][x], mode));
        }
    }
}


void parse_parameters(string &command, std::vector<string> &parameter_vector)
{

    // Remove leading and trailing white spaces
    boost::trim(command);

    // Split command at white spaces, multiple successive spaces will be compressed
    parameter_vector.clear();
    boost::split(parameter_vector, command, boost::is_space(), boost::token_compress_on);

    parameter_vector.erase(parameter_vector.begin(), parameter_vector.begin() + 1);
}


/* ================================================================================================================== */


void handle_quit(string &)
{
    std::cout << "Exiting." << std::endl;
}


void handle_help(string &command)
{
    std::vector<string> params;
    parse_parameters(command, params);

    if (params.empty()) {
        std::cout << MAIN_HELP << std::endl;
    } else {
        boost::to_lower(params[0]);

        if (help_map.find(params[0]) != help_map.end()) {
            std::cout << help_map.at(params[0]) << std::endl;
        } else {
            std::cout << "Invalid parameter for command \"help\"." << std::endl << std::endl << MAIN_HELP << std::endl;
        }
    }
}


void handle_set(string &command)
{
    std::vector<string> params;
    parse_parameters(command, params);

    Mode mode = (params[0] == "picross") ? picross : normal;
    LevelSide left_side, right_side;

    do {
        std::cout << "Please enter the left side of the level you want to solve. "
                  << "Once you're done, press <ENTER> twice." << std::endl;
        parse_level_side(left_side, mode);

        std::cout << "Please enter the right side of the level you want to solve." << std::endl;
        parse_level_side(right_side, mode);

        if (left_side.height != right_side.height) {
            std::cout << "Both sides of the level must have the same height." << std::endl;
        }

    } while (left_side.height != right_side.height);

    delete current_level;
    current_level = new Level(mode, left_side, right_side);
}


void handle_get(string &command)
{
    std::vector<string> params;
    parse_parameters(command, params);

    int index = parse_stack_index(params);

    if (index != -1) {
        delete current_level;
        current_level = new Level(level_stack.vector.at((u_long) index).level);
    }
}


void handle_push(string &command)
{
    std::vector<string> params;
    parse_parameters(command, params);

    if (current_level == nullptr) {
        std::cout << "There is no level currently active." << std::endl;
        return;
    }

    if (params.empty()) {
        params.emplace_back("");
    }

    if (!params[0].empty()) {
        for (const auto &entry : level_stack.vector) {
            if (entry.name == params[0]) {
                std::cout << "There is already a level with the specified name on the level stack" << std::endl;
                return;
            }
        }
    }

    level_stack.vector.emplace(level_stack.vector.begin(), level_stack.next_id++, params[0], Level(*current_level));
}


void handle_pop(string &)
{
    if (level_stack.vector.empty()) {
        std::cout << "There is no level to be removed from the stack." << std::endl;
        return;
    }

    delete current_level;
    current_level = new Level(level_stack.vector.at(0).level);
    level_stack.vector.erase(level_stack.vector.begin(), level_stack.vector.begin() + 1);
}


void handle_unset(string &)
{
    delete current_level;
    current_level = nullptr;
}


void handle_delete(string &command)
{
    std::vector<string> params;
    parse_parameters(command, params);

    if (params.empty()) {
        std::cout << "There is no entry of the stack specified." << std::endl;
        return;
    }

    int index = parse_stack_index(params);

    if (index != -1) {
        level_stack.vector.erase(level_stack.vector.begin() + index, level_stack.vector.begin() + index + 1);
    }
}


void handle_clear(string &)
{
    level_stack.vector.clear();
    level_stack.next_id = 0;
}


void handle_print(string &command)
{
    std::vector<string> params;
    parse_parameters(command, params);

    if (params.empty()) {
        if (current_level == nullptr) {
            std::cout << "There is no level currently active." << std::endl;
            return;
        }

        print(*current_level);
    } else {
        int index = parse_stack_index(params);

        if (index == -1)
            return;

        print(level_stack.vector.at((u_long) index));
    }
}


void handle_list(string &)
{
    print(level_stack);
}


void handle_solve(string &)
{
    if (current_level == nullptr) {
        std::cout << "There is no level currently active." << std::endl;
        return;
    }

    bool success = false;
    Solution solution;

    if (current_level->mode == normal) {
        std::cout << "Solving in min mode." << std::endl;
        success = solve_min(*current_level, solution);
    } else if (current_level->mode == picross) {
        std::cout << "Solving in picross mode." << std::endl;
        success = solve_picross(*current_level, solution);
    }

    if (!success) {
        std::cout << "There has been an error while solving the level." << std::endl;
        return;
    }

    print(solution);
}


void handle_solve_min(string &)
{
    if (current_level == nullptr) {
        std::cout << "There is no level currently active." << std::endl;
        return;
    }

    Solution solution;

    if (!solve_min(*current_level, solution)) {
        std::cout << "There has been an error while solving the level." << std::endl;
        return;
    }

    print(solution);
}


void handle_solve_max(string &)
{
    if (current_level == nullptr) {
        std::cout << "There is no level currently active." << std::endl;
        return;
    }

    Solution solution;

    if (!solve_max(*current_level, solution)) {
        std::cout << "There has been an error while solving the level." << std::endl;
        return;
    }

    print(solution);
}


void handle_solve_picross(string &)
{
    if (current_level == nullptr) {
        std::cout << "There is no level currently active." << std::endl;
        return;
    }

    if (current_level->mode != picross) {
        std::cout << "The currently active level doesn't have picross constraints." << std::endl;
        return;
    }


    Solution solution;

    if (!solve_picross(*current_level, solution)) {
        std::cout << "There has been an error while solving the level." << std::endl;
        return;
    }

    print(solution);
}


/* ================================================================================================================== */


const handler_map_type handler_map = {  // NOLINT(cert-err58-cpp)
        {QUIT,          handle_quit},
        {HELP,          handle_help},
        {SET,           handle_set},
        {GET,           handle_get},
        {PUSH,          handle_push},
        {POP,           handle_pop},
        {UNSET,         handle_unset},
        {DELETE,        handle_delete},
        {CLEAR,         handle_clear},
        {PRINT,         handle_print},
        {LIST,          handle_list},
        {SOLVE,         handle_solve},
        {SOLVE_MIN,     handle_solve_min},
        {SOLVE_MAX,     handle_solve_max},
        {SOLVE_PICROSS, handle_solve_picross},
};


void handle_command(const string &command)
{
    string copy = boost::trim_left_copy(command) + " ";

    for (const auto &[key, handler] : handler_map) {
        if (boost::istarts_with(copy, key + " ")) {
            return handler(copy);
        }
    }

    std::cout << "The specified command doesn't exist." << std::endl;
}


bool quit_after(string &command)
{
    return boost::trim_copy(command) == QUIT;
}


void read_input(string &input_string)
{
    std::cout << "> ";

    input_string.erase();
    std::getline(std::cin, input_string);
}


int main(int argc, char *argv[])
{
    for (bool quit = false; !quit;) {
        string input;
        read_input(input);

        handle_command(input);

        quit = quit_after(input);
    }
}
