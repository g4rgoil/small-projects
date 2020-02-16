// #deI//fine GLEW_STATIC

#include <iostream>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include "application.hpp"
#include "conway/conways-game.hpp"
#include "conway-gpu/conways-game.hpp"

Application::Application(sf::RenderWindow *window, AbstractGame *game) :
    window_(window), game_(game), fps_counter_(),
    framerate_limit_(WINDOW_FRAMERATE), show_hud_(true)
{
    updateFramerateLimit(framerate_limit_);

    // sf::View view({(float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT / 2}, {(float)WINDOW_WIDTH, (float)WINDOW_HEIGHT});
    // window_.setView(view);
}

int Application::mainloop()
{
    sf::VertexBuffer quad(sf::PrimitiveType::Quads);
    sf::Vertex vertices[4] = {{{0.f, 0.f}, {0.f, (float)game_->getSize().y}},
        {{WINDOW_WIDTH, 0.f}, {(float)game_->getSize().x, (float)game_->getSize().y}},
        {{WINDOW_WIDTH, WINDOW_HEIGHT}, {(float)game_->getSize().x, 0.f}},
        {{0.f, WINDOW_HEIGHT}, {0.f, 0.f}}};
    quad.create(4);
    quad.update(vertices);

    while (window_->isOpen()) {
        pollEvents();

        window_->resetGLStates();
        window_->clear();

        game_->draw(window_, quad);
        if (show_hud_) {
            fps_counter_.draw(*window_);
        }

        window_->display();

        game_->nextGeneration();
        fps_counter_.update();
    }

    return EXIT_SUCCESS;
}

void Application::pollEvents()
{
    sf::Event event;
    while (window_->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_->close();
        } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) {
            framerate_limit_ = updateFramerateLimit(framerate_limit_ + 1);
        } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
            framerate_limit_ = updateFramerateLimit(framerate_limit_ - 1);
        } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::H) {
            show_hud_ = !show_hud_;
        }
    }
}

uint Application::updateFramerateLimit(int framerate)
{
    framerate = std::max(1, framerate);
    window_->setFramerateLimit(framerate);
    return framerate;
}

int main(int argc, char *argv[])
{
    sf::RenderWindow window({WINDOW_WIDTH, WINDOW_HEIGHT}, "Cellular Automaton",
                            sf::Style::Titlebar | sf::Style::Close);
    glewInit();
    conway::ConwaysGame game(GRID_DIM_X, GRID_DIM_Y);
    Application application(&window, &game);


    return application.mainloop();
}