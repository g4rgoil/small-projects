#ifndef ABSTRACT_GAME_HPP_INCLUDED
#define ABSTRACT_GAME_HPP_INCLUDED

#include <SFML/System/Vector2.hpp>

class AbstractGame
{
public:
    virtual sf::Vector2u getSize() const = 0;
    virtual void draw(uint vertex_array) = 0;
    virtual void nextGeneration() = 0;
};

#endif  // ABSTRACT_GAME_HPP_INCLUDED