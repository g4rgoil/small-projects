#ifndef ABSTRACT_GAME_HPP_INCLUDED
#define ABSTRACT_GAME_HPP_INCLUDED

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>

class AbstractGame
{
public:
    virtual sf::Vector2<size_t> getSize() const = 0;
    virtual void draw(sf::RenderTarget *target, const sf::Drawable &drawable) const = 0;
    virtual void nextGeneration() = 0;
};

#endif  // ABSTRACT_GAME_HPP_INCLUDED