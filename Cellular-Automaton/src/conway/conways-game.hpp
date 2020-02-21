#ifndef CONWAY_CONWAYS_GAME_HPP_INCLUDED
#define CONWAY_CONWAYS_GAME_HPP_INCLUDED

#include <boost/numeric/ublas/matrix.hpp>
#include <SFML/System/Vector2.hpp>

#include "../abstract_game.hpp"


using namespace boost::numeric;

namespace conway {

enum State : char {dead = 0, alive = 1};

class ConwaysGame: public AbstractGame
{
public:
    ConwaysGame(uint size_x, uint size_y);
    virtual sf::Vector2u getSize() const;
    virtual void draw(uint vertex_array);
    virtual void nextGeneration();

private:
    const uint size_x_;
    const uint size_y_;

    ublas::matrix<State> current_grid;
    ublas::matrix<State> alternate_grid;

    uint texture_;
    uint shader_program_;
    bool is_dirty_;

    static State nextState(State state, int neighbours);
    int countNeighbours(int x, int y) const;
    void writeToTexture(unsigned int texture);
};

}

#endif  // CONWAY_CONWAYS_GAME_HPP_INCLUDED