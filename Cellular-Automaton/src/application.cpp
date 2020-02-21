#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SFML/Graphics.hpp>

#include "application.hpp"
#include "conway/conways-game.hpp"
// #include "conway-gpu/conways-game.hpp"

Application::Application(GLFWwindow *window, AbstractGame *game) :
        window_(window), game_(game), fps_counter_(WINDOW_WIDTH, WINDOW_HEIGHT),
        framerate_limit_(WINDOW_FRAMERATE), show_hud_(true)
{
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, Application::key_callback);

    float vertices[] = {
        -1.0f,  1.0f, 0.0f, 1.0f,   // top left
        -1.0f, -1.0f, 0.0f, 0.0f,   // bottom left
         1.0f,  1.0f, 1.0f, 1.0f,   // top right

         1.0f,  1.0f, 1.0f, 1.0f,   // top right
        -1.0f, -1.0f, 0.0f, 0.0f,   // bottom left
         1.0f, -1.0f, 1.0f, 0.0f,   // bottom right
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

int Application::mainloop()
{
    while (!glfwWindowShouldClose(window_)) {
        glClear(GL_COLOR_BUFFER_BIT);

        game_->draw(vertex_array_);

        if (show_hud_) {
            fps_counter_.draw();
        }

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
    std::cout << description << std::endl;
}

int main(int argc, char *argv[])
{
    if (!glfwInit()) {
        std::cout << "GLFW initialisation failed" << std::endl;
        return EXIT_FAILURE;
    }

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
    // glfwSwapInterval(1);  // enable Vsync
    glfwSwapInterval(0);

    conway::ConwaysGame game(GRID_DIM_X, GRID_DIM_Y);
    Application application(window, &game);
    int exit_code = application.mainloop();

    glfwTerminate();
    return exit_code;
}