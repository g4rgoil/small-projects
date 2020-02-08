#include "fps_counter.hpp"

#include <string>
#include <cmath>

FpsCounter::FpsCounter() :
    clock_(), current_fps_(0)
{
    if (!font_.loadFromFile("/usr/share/fonts/TTF/DejaVuSans.ttf")) {
        // put font into a seperate class? or give as parameter?
    }

    text_.setFont(font_);
    text_.setString("0");
    text_.setFillColor(sf::Color::Green);
    text_.setCharacterSize(40);
    text_.setStyle(sf::Text::Bold);
}

void FpsCounter::update()
{
    const float weight = 0.3f;

    int frame_time = clock_.restart().asMilliseconds();
    current_fps_ = 1000 * weight / frame_time + current_fps_ * (1.0f - weight);
    text_.setString(std::to_string((int)std::round(current_fps_)));
}

int FpsCounter::currentFps() const
{
    return current_fps_;
}

void FpsCounter::draw(sf::RenderTarget &target) const
{
    target.draw(text_);
}

