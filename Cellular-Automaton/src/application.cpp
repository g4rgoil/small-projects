#include <iostream>
#include <exception>
#include <memory>
#include <algorithm>
#include <cstring>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "application.hpp"
#include "exceptions.hpp"
#include "conway/game.hpp"
#include "conway/game-gpu.hpp"


Application::Application(GLFWwindow *window, AbstractGame *game) :
        window_(window), game_(game),
        fps_counter_(WINDOW_WIDTH, WINDOW_HEIGHT),
        fps_limiter_(0.0f),
        view_port_(game->getSize()),
        show_hud_(true)
{
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        ((Application*)glfwGetWindowUserPointer(window))->key_callback(key, scancode, action, mods);
    });
    glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button, int action, int mods) {
        ((Application*)glfwGetWindowUserPointer(window))->mouse_button_callback(button, action, mods);
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xpos, double ypos) {
        ((Application*)glfwGetWindowUserPointer(window))->cursor_position_callback(xpos, ypos);
    });
    glfwSetScrollCallback(window_, [](GLFWwindow *window, double xoffset, double yoffset) {
        ((Application*)glfwGetWindowUserPointer(window))->scroll_callback(xoffset, yoffset);
    });

    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);
    glm::uvec2 game_size = game->getSize();

    vertices_ = {{
        { 0.0f,                0.0f,                  0.0f,               0.0f               },  // bottom left
        { (float)window_width, (float)window_height,  (float)game_size.x, (float)game_size.y },  // top right
        { 0.0f,                (float)window_height,  0.0f,               (float)game_size.y },  // top left
 
        { (float)window_width, (float)window_height,  (float)game_size.x, (float)game_size.y },  // top right
        { 0.0f,                0.0f,                  0.0f,               0.0f               },  // bottom left
        { (float)window_width, 0.0f,                  (float)game_size.x, 0.0f               },  // bottom right
    }};

    glGenVertexArrays(1, &vertex_array_);
    glGenBuffers(1, &vertex_buffer_);

    glBindVertexArray(vertex_array_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

Application::~Application()
{
    glDeleteBuffers(1, &vertex_buffer_);
    glDeleteVertexArrays(1, &vertex_array_);
}

int Application::mainloop()
{
    while (!glfwWindowShouldClose(window_)) {
        glClear(GL_COLOR_BUFFER_BIT);

        game_->draw(vertex_array_);

        if (show_hud_) fps_counter_.draw();

        game_->nextGeneration();
        fps_counter_.update();
        fps_limiter_.wait();

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }

    return EXIT_SUCCESS;
}

void Application::key_callback(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_H && action == GLFW_PRESS) {
        show_hud_ = !show_hud_;
    } else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
    } else if (key == GLFW_KEY_RIGHT_BRACKET && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        view_port_.resize({-1.0f, -1.0f});
    } else if (key == GLFW_KEY_SLASH && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        view_port_.resize({1.0f, 1.0f});
    } else if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        view_port_.move({-1.0f, 0.0f});
    } else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        view_port_.move({1.0f, 0.0f});
    } else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        view_port_.move({0.0f, -1.0f});
    } else if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        view_port_.move({0.0f, 1.0f});
    }

    updateVertexArray(view_port_.bottom_left(), view_port_.top_right());
}

void Application::mouse_button_callback(int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        double xpos, ypos;
        glfwGetCursorPos(window_, &xpos, &ypos);
        last_cursor_pos_ = glm::vec2(xpos, ypos);
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void Application::cursor_position_callback(double xpos, double ypos)
{
    if (glfwGetInputMode(window_, GLFW_CURSOR) == GLFW_CURSOR_DISABLED){
        int width, height;
        glfwGetWindowSize(window_, &width, &height);
        glm::vec2 window_size(width, height);

        glm::vec2 delta = glm::vec2(xpos, ypos) - last_cursor_pos_;
        last_cursor_pos_ = glm::vec2(xpos, ypos);

        glm::vec2 scale = view_port_.size() / window_size;
        view_port_.move(glm::vec2(-1.0f, 1.0f) * delta * scale);
        updateVertexArray(view_port_.bottom_left(), view_port_.top_right());
    }
}

void Application::scroll_callback(double xoffset, double yoffset)
{
    view_port_.resize(-1.0f * glm::vec2(yoffset));
    updateVertexArray(view_port_.bottom_left(), view_port_.top_right());
}

void Application::updateVertexArray(glm::vec2 bottom_left, glm::vec2 top_right)
{
    vertices_[0][2] = vertices_[2][2] = vertices_[4][2] = bottom_left.x;
    vertices_[1][2] = vertices_[3][2] = vertices_[5][2] = top_right.x;
    vertices_[0][3] = vertices_[4][3] = vertices_[5][3] = bottom_left.y;
    vertices_[1][3] = vertices_[2][3] = vertices_[3][3] = top_right.y;

    glBindVertexArray(vertex_array_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_.data(), GL_STATIC_DRAW);
}

void error_callback(int error, const char *description)
{
    std::cout << "Error occurred in GLFW window, error_code=" << error << std::endl;
    std::cout << description << std::endl;
}

int main(int argc, char *argv[])
{
    if (!glfwInit()) {
        std::cout << "GLFW initialisation failed" << std::endl;
        return EXIT_FAILURE;
    }

    // Only needed for uncaught exceptions (or simillar situtations)
    std::set_terminate([]() { glfwTerminate(); });

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Cellular Automaton", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return EXIT_FAILURE; 
    }

    glfwSetErrorCallback(error_callback);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(0);

    glm::mat4 projection = glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT);
    std::unique_ptr<AbstractGame> game;

    try {
        game = std::make_unique<conway::ConwaysGameGpu>(GRID_DIM_X, GRID_DIM_Y, projection);
    } catch (const OpenGlError &e) {
        std::cout << "OpenGL caused an exception:" << std::endl;
        std::cout << e.what() << std::endl;

        glfwTerminate();
        return EXIT_FAILURE;
    }

    Application application(window, game.get());

    int exit_code = application.mainloop();
    glfwTerminate();
    return EXIT_SUCCESS;
}