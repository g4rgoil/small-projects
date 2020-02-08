#include "application.hpp"

#include "conway/universe.hpp"
#include <iostream>

Application::Application(AbstractGame *game) :
    window_({WINDOW_WIDTH, WINDOW_HEIGHT}, "Cellular Automaton", sf::Style::Titlebar | sf::Style::Close),
    game_(game), fps_counter_(), framerate_limit_(WINDOW_FRAMERATE), show_hud_(true)
{
    updateFramerateLimit(framerate_limit_);

    // sf::View view({(float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT / 2}, {(float)WINDOW_WIDTH, (float)WINDOW_HEIGHT});
    // window_.setView(view);
}

int Application::mainloop()
{
    sf::Image image;
    image.create(game_->sizeX(), game_->sizeY(), sf::Color::White);

    sf::Texture texture;
    texture.create(game_->sizeX(), game_->sizeY());
    sf::RenderStates state(&texture);

    sf::VertexArray quad(sf::Quads, 4);
    quad[0] = {{0.f, 0.f}, {0.f, (float)game_->sizeY()}};
    quad[1] = {{WINDOW_WIDTH, 0.f}, {(float)game_->sizeX(), (float)game_->sizeY()}};
    quad[2] = {{WINDOW_WIDTH, WINDOW_HEIGHT}, {(float)game_->sizeX(), 0.f}};
    quad[3] = {{0.f, WINDOW_HEIGHT}, {0.f, 0.f}};

    while (window_.isOpen()) {
        pollEvents();

        game_->makeImage(image);

        texture.update(image);

        window_.clear();
        window_.draw(quad, state);

        if (show_hud_) {
            fps_counter_.draw(window_);
        }

        window_.display();

        game_->nextGeneration();
        fps_counter_.update();
    }

    return EXIT_SUCCESS;
}

void Application::pollEvents()
{
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
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
    window_.setFramerateLimit(framerate);
    return framerate;
}

int main(int argc, char const *argv[])
{
    Universe game(GRID_DIM_X, GRID_DIM_Y);
    Application application(&game);

    return application.mainloop();
}