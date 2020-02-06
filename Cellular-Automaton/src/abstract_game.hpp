#ifndef ABSTRACT_GAME_HPP_INCLUDED
#define ABSTRACT_GAME_HPP_INCLUDED

#include <SFML/Graphics.hpp>

class AbstractGame
{
public:
    virtual size_t sizeX() const = 0;
    virtual size_t sizeY() const = 0;
    virtual void makeImage(sf::Image &image) const = 0;
    virtual void nextGeneration() = 0;
};

#endif  // ABSTRACT_GAME_HPP_INCLUDED