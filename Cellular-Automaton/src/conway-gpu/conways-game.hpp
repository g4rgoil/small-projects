#ifndef CONWAY_GPU_CONWAYS_GAME_HPP_INCLUDED
#define CONWAY_GPU_CONWAYS_GAME_HPP_INCLUDED

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>
#include <GL/glew.h>
#include "../abstract_game.hpp"

namespace conway_gpu {

class ConwaysGame: public AbstractGame
{
public:
    ConwaysGame(size_t dim_x, size_t dim_y);
    virtual sf::Vector2<size_t> getSize() const;
    virtual void draw(sf::RenderTarget *target, const sf::Drawable &drawable);
    virtual void nextGeneration();

private:
    const size_t size_x_;
    const size_t size_y_;

    sf::Texture textures_[2];

    sf::Texture *current_texture_;
    sf::Texture *alternate_texture_;

    sf::Shader draw_shader_;

    GLuint update_program_;
};

}
    
#endif  // CONWAY_GPU_CONWAYS_GAME_HPP_INCLUDED