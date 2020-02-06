#ifndef CONWAY_UNIVERSE_HPP_INCLUDED
#define CONWAY_UNIVERSE_HPP_INCLUDED

#include <boost/numeric/ublas/matrix.hpp>
#include <SFML/Graphics.hpp>
#include "../abstract_game.hpp"

using namespace boost::numeric;

enum State {dead = 0, alive = 1};

class Universe: public AbstractGame
{
public:
    Universe(size_t dim_x, size_t dim_y);
    virtual size_t sizeX() const;
    virtual size_t sizeY() const;
    virtual void makeImage(sf::Image &image) const;
    virtual void nextGeneration();

private:
    const size_t size_x;
    const size_t size_y;

    ublas::matrix<State> cell_grid;
    ublas::matrix<int> neighbour_grid;

    static State nextCellState(State state, int neighbours);
    void increaseNeighbourhood(int x, int y);

};

#endif  // CONWAY_UNIVERSE_HPP_INCLUDED