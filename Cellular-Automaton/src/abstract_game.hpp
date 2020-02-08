#ifndef ABSTRACT_GAME_HPP_INCLUDED
#define ABSTRACT_GAME_HPP_INCLUDED

#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Vector2.hpp>

class AbstractGame
{
public:
    virtual sf::Vector2<size_t> getSize() const = 0;
    virtual void makeImage(sf::Image &image) const = 0;
    virtual void nextGeneration() = 0;
};

#endif  // ABSTRACT_GAME_HPP_INCLUDED