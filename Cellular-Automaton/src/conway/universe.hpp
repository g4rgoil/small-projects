#ifndef CONWAY_UNIVERSE_HPP_INCLUDED
#define CONWAY_UNIVERSE_HPP_INCLUDED

#include <boost/numeric/ublas/matrix.hpp>
#include <SFML/System/Vector2.hpp>
#include "../abstract_game.hpp"

using namespace boost::numeric;

enum State {dead = 0, alive = 1};

class Universe: public AbstractGame
{
public:
    Universe(size_t dim_x, size_t dim_y);
    virtual sf::Vector2<size_t> getSize() const;
    virtual void makeImage(sf::Image &image) const;
    virtual void nextGeneration();

private:
    const size_t size_x;
    const size_t size_y;

    ublas::matrix<State> current_grid;
    ublas::matrix<State> alternate_grid;

    static State nextState(State state, int neighbours);
    int countNeighbours(int x, int y) const;

};

#endif  // CONWAY_UNIVERSE_HPP_INCLUDED