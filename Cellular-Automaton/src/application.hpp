#ifndef APPLICATION_HPP_INCLUDED
#define APPLICATION_HPP_INCLUDED

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>

#include "abstract_game.hpp"
#include "fps_counter.hpp"
#include "fps_limiter.hpp"
#include "view_port.hpp"


const uint WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 1024;
const size_t GRID_DIM_X = 128, GRID_DIM_Y = 128;

using vertex = std::array<float, 4>;

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
    FpsLimiter fps_limiter_;
    ViewPort view_port_;

    uint vertex_buffer_;
    uint vertex_array_;
    bool show_hud_;
    glm::vec2 last_cursor_pos_;
    std::array<vertex, 6> vertices_;


    void key_callback(int key, int scancode, int action, int mods);
    void mouse_button_callback(int button, int action, int mods);
    void cursor_position_callback(double xpos, double ypos);
    void scroll_callback(double xoffset, double yoffset);
    void updateVertexArray(glm::vec2 bottom_left, glm::vec2 top_right);
};

#endif  // APPLICATION_HPP_INCLUDED
