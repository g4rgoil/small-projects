#ifndef APPLICATION_HPP_INCLUDED
#define APPLICATION_HPP_INCLUDED

#include <GLFW/glfw3.h>

#include "abstract_game.hpp"
#include "fps_counter.hpp"

const uint WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 1024;
const size_t GRID_DIM_X = 128, GRID_DIM_Y = 128;

class Application
{
  public:
    Application(GLFWwindow *window, AbstractGame *game);
    Application(const Application&) = delete;
    Application(Application&&) = default;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = default;
    ~Application();

    int mainloop();

  private:
    GLFWwindow *window_;
    AbstractGame *game_;
    FpsCounter fps_counter_;
    uint vertex_buffer_;
    uint vertex_array_;
    bool show_hud_;

    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void key_callback(int key, int scancode, int action, int mods);
};

#endif  // APPLICATION_HPP_INCLUDED
