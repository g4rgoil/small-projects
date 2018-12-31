//
// Created by pascal on 30.12.18.
//

#ifndef INC_3D_PUZZLE_PUZZLE_HPP
#define INC_3D_PUZZLE_PUZZLE_HPP


#include <vector>
#include <map>
#include <string>


#define QUIT "quit"
#define HELP "help"
#define SET "set"
#define GET "get"
#define PUSH "push"
#define POP "pop"
#define UNSET "unset"
#define DELETE "delete"
#define CLEAR "clear"
#define PRINT "print"
#define LIST "list"
#define SOLVE "solve"
#define SOLVE_MIN "solve-min"
#define SOLVE_MAX "solve-max"
#define SOLVE_PICROSS "solve-picross"


typedef std::string string;

typedef void (*handler)(string &);

typedef std::map<string, string> help_map_type;
typedef std::map<string, handler> handler_map_type;

typedef std::vector<std::vector<int>> level_vector;
typedef std::vector<std::vector<bool>> solution_vector;

typedef std::vector<int> level_line;
typedef std::vector<bool> solution_line;


enum Mode
{
    normal,
    picross
};


struct LevelSide
{
    u_long width;   // = x
    u_long height;  // = y
    level_vector vector;

    LevelSide();

    LevelSide(u_long width, u_long height, level_vector &vector);

    LevelSide(const LevelSide &other);

    int at(u_long x, u_long y) const;

    void assign(u_long x, u_long y, int value);
};


struct Level
{
    Mode mode;

    LevelSide left;
    LevelSide right;

    Level(Mode mode, const LevelSide &left, const LevelSide &right);

    Level(const Level &other);
};


struct Solution
{
    u_long width;      // = x
    u_long depth;      // = y
    u_long height;     // = z

    solution_vector vector;

    Solution();

    bool at(u_long x, u_long y, u_long z) const;

    void assign(u_long x, u_long y, u_long z, bool value);
};


struct LevelStackEntry
{
    int id;
    string name;
    Level level;

    LevelStackEntry(int id, string &name, const Level &level);
};


struct LevelStack
{
    int next_id;
    std::vector<LevelStackEntry> vector;

    LevelStack() noexcept;
};


#endif //INC_3D_PUZZLE_PUZZLE_HPP
