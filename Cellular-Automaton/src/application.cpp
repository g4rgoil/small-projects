#include "application.hpp"

#include "conway/universe.hpp"

Application::Application(AbstractGame &_game) :
    window({WINDOW_WIDTH, WINDOW_HEIGHT}, "Cellular Automaton", sf::Style::Titlebar | sf::Style::Close),
    game(_game)
{
    window.setFramerateLimit(15);

    sf::View view({(float)game.sizeX() / 2, (float)game.sizeY() / 2}, {(float)game.sizeX(), (float)game.sizeY()});
    window.setView(view);
}

int Application::mainloop()
{
    sf::Image image;
    image.create(game.sizeX(), game.sizeY(), sf::Color::Black);

    while (window.isOpen()) {
        pollEvents();

        game.makeImage(image);

        sf::Texture texture;
        texture.loadFromImage(image);
        sf::Sprite sprite(texture);

        window.clear();
        window.draw(sprite);
        window.display();

        game.nextGeneration();
    }

    return EXIT_SUCCESS;
}

void Application::pollEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

int main(int argc, char const *argv[])
{
    Universe game(GRID_DIM_X, GRID_DIM_Y);
    Application application(game);

    return application.mainloop();
}