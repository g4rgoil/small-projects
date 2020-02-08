#ifndef CONWAY_CONWAYS_GAME_HPP_INCLUDED
#define CONWAY_CONWAYS_GAME_HPP_INCLUDED

#include <boost/numeric/ublas/matrix.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "../abstract_game.hpp"

using namespace boost::numeric;

enum State {dead = 0, alive = 1};

class ConwaysGame: public AbstractGame
{
public:
    ConwaysGame(size_t dim_x, size_t dim_y);
    virtual sf::Vector2<size_t> getSize() const;
    virtual void draw(sf::RenderTarget *target, const sf::Drawable &drawable) const;
    virtual void nextGeneration();

private:
    const size_t size_x;
    const size_t size_y;

    ublas::matrix<State> current_grid;
    ublas::matrix<State> alternate_grid;

    sf::Image image_;
    sf::Texture texture_;
    sf::RenderStates state_;

    static State nextState(State state, int neighbours);
    int countNeighbours(int x, int y) const;
    void makeImage(sf::Image *image) const;
    void makeTexture(sf::Texture *texture) const;
};

#endif  // CONWAY_CONWAYS_GAME_HPP_INCLUDED