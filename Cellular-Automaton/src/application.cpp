#include <iostream>
#include <exception>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "application.hpp"
#include "exceptions.hpp"
#include "conway/game.hpp"
#include "conway/game-gpu.hpp"

Application::Application(GLFWwindow *window, AbstractGame *game) :
        window_(window), game_(game),
        fps_counter_(WINDOW_WIDTH, WINDOW_HEIGHT),
        show_hud_(true)
{
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, Application::key_callback);

    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);
    glm::uvec2 game_size = game->getSize();

    float vertices[] = {
        0.0f,                0.0f,                  0.0f,               0.0f,                // bottom left
        (float)window_width, (float)window_height,  (float)game_size.x, (float)game_size.y,  // top right
        0.0f,                (float)window_height,  0.0f,               (float)game_size.y,  // top left

        (float)window_width, (float)window_height,  (float)game_size.x, (float)game_size.y,  // top right
        0.0f,                0.0f,                  0.0f,               0.0f,                // bottom left
        (float)window_width, 0.0f,                  (float)game_size.x, 0.0f                 // bottom right
    };

    glGenVertexArrays(1, &vertex_array_);
    glGenBuffers(1, &vertex_buffer_);

    glBindVertexArray(vertex_array_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
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

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }

    return EXIT_SUCCESS;
}

void Application::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    ((Application*)glfwGetWindowUserPointer(window))->key_callback(key, scancode, action, mods);
}

void Application::key_callback(int key, int scancode, int action, int mods)
{
    if (key = GLFW_KEY_H && action == GLFW_PRESS) {
        show_hud_ = !show_hud_;
    }
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
    glfwSwapInterval(1);  // enable Vsync

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