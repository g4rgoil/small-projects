//
// Created by pascal on 28.12.18.
//

#ifndef INC_3D_PUZZLE_HELP_HPP
#define INC_3D_PUZZLE_HELP_HPP


const char *MAIN_HELP =
        "List of available commands:\n\n"
        "quit          - Terminate the application.\n"
        "set           - Query the user for a new level, and replace the currently\n"
        "                active on with it.\n"
        "get           - Replace the currently active level with the specified one\n"
        "                from the stack.\n"
        "push          - Add the currently active level to the top of the stack.\n"
        "pop           - Remove the newest level from the stack and replace the\n"
        "                currently active one with it.\n"
        "unset         - Remove the currently active level.\n"
        "delete        - Remove the specified level from the stack.\n"
        "clear         - Remove all levels from the stack.\n"
        "print         - Print a level to the terminal.\n"
        "list          - List all levels stored on the stack.\n"
        "solve         - Solve the currently active level.\n"
        "solve-min     - Solve the currently active level with the minimal amount of\n"
        "                blocks possible.\n"
        "solve-max     - Solve the currently active level with the maximum amount of\n"
        "                blocks possible.\n"
        "solve-picross - Solve the currently active level in a way, satisfying the\n"
        "                specified picross constraints\n"
        "                (only available if the level has picross constraints).\n\n"
        "Type \"help\" followed by a command name, for more information on that command.\n";


const char *HELP_QUIT = "Terminate the application (discards all levels).\n";


const char *HELP_SET =
        "Query the user for a new level and replace the currently active on with it.\n"
        "Allows the user to specify, whether the level should be created in normal or in\n"
        "picross mode. The command will default to normal mode, if neither is specified.\n\n"
        "There are the following restrictions on the dimensions on the dimensions of a level:\n"
        "  - The size of every dimension must be at least 1\n"
        "  - The height of the left and right side must be equal\n"
        "  - Both sides must be rectangular\n"
        "usage: set [(normal | picross)]\n";


const char *HELP_GET =
        "Replace the currently active level with the specified one from the stack. Allows\n"
        "the user to specify the level he wants to use. The level may be specified, using\n"
        "its unique numerical identifier or it's name, if one exists. The command will\n"
        "default to the newest level on the stack, if none is specified.\n\n"
        "usage: get [<level>]\n";


const char *HELP_PUSH =
        "Add the currently active level to the top of the stack. Allows the user to\n"
        "specify an optional name, used to identify the level. The name of a level is"
        "case insensitive (i.e. the case of a character is irrelevant).\n\n"
        "usage: push [<name>]\n";


const char *HELP_POP =
        "Remove the newest level from the stack (i.e. the last level, pushed to the\n"
        "stack) and replace the currently active one with it. It is the users\n"
        "responsibility, to save the level that is being replaced by this command, if\n"
        "necessary.\n";


const char *HELP_UNSET = "Remove the currently active level.\n";


const char *HELP_DELETE =
        "Remove the specified level from the stack. Requires the user to specify the\n"
        "level to be removed from the stack. The level may be specified, using its\n"
        "unique numerical identifier or it's name, if one exists. \n\n"
        "usage: delete <level>\n";


const char *HELP_CLEAR = "Remove all levels from the stack.\n";


const char *HELP_PRINT =
        "Print a level to the terminal. Allows the user the specify the level, that is\n"
        "to be printed. The command will default to the currently active level, if none\n"
        "is specified. A level may be specified, using its unique numerical identifier\n"
        "or it's name, if one exists.\n\n"
        "usage: print [<level>]\n";


const char *HELP_LIST =
        "List all levels, currently stored on the stack.\n";


const char *HELP_SOLVE =
        "Solve the currently active level. If the level has picross constraints, it\n"
        "will be solved in a way, satisfying these constraints. Otherwise it will be\n"
        "solved, using the minimal possible number of blocks.\n";


const char *HELP_SOLVE_MIN =
        "Solve the currently active level, using the minimal possible number of blocks.\n";


const char *HELP_SOLVE_MAX =
        "Solve the currently active level, using the maximal possible number of blocks.\n";


const char *HELP_SOLVE_PICROSS =
        "Solve the currently active level in a way, satisfying the specified picross\n"
        "constraints. This command is only available, if the level has picross constrains.\n";


#endif //INC_3D_PUZZLE_HELP_HPP
