#ifndef FPS_COUNTER_HPP_INCLUDED
#define FPS_COUNTER_HPP_INCLUDED

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Clock.hpp>

class FpsCounter 
{
public:
    FpsCounter();
    void update();
    int currentFps() const;
    void draw(sf::RenderTarget &target) const;

private:
    float current_fps_;
    sf::Clock clock_;
    sf::Text text_;
    sf::Font font_;
};

#endif  // FPS_COUNTER_HPP_INCLUDED