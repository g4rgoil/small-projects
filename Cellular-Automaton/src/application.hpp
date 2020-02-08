#ifndef APPLICATION_HPP_INCLUDED
#define APPLICATION_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include "abstract_game.hpp"
#include "fps_counter.hpp"

const uint WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 1024;
const uint WINDOW_FRAMERATE = 60;
const size_t GRID_DIM_X = 128, GRID_DIM_Y = 128;

class Application
{
public:
    Application(AbstractGame *game);
    int mainloop();

private:
    sf::RenderWindow window_;
    AbstractGame *game_;
    FpsCounter fps_counter_;
    uint framerate_limit_;
    bool show_hud_;

    void pollEvents();
    uint updateFramerateLimit(int framerate);
};

#endif  // APPLICATION_HPP_INCLUDED
