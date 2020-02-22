#ifndef CONWAY_GAME_HPP_INCLUDED
#define CONWAY_GAME_HPP_INCLUDED

#include <boost/numeric/ublas/matrix.hpp>
#include <glad/glad.h>

#include "../abstract_game.hpp"
#include "renderer.hpp"


using namespace boost::numeric;

namespace conway {

enum State : char {dead = 0, alive = 1};

class ConwaysGame: public AbstractGame
{
  public:
    ConwaysGame(uint size_x, uint size_y);
    virtual void draw(GLuint vertex_array);
    virtual void nextGeneration();

  private:
    const uint size_x_;
    const uint size_y_;

    ublas::matrix<State> current_grid;
    ublas::matrix<State> alternate_grid;

    Renderer renderer_;
    uint texture_;
    bool is_dirty_;

    static State nextState(State state, int neighbours);
    int countNeighbours(int x, int y) const;
    void writeToTexture(GLuint texture);
};

}

#endif  // CONWAY_GAME_HPP_INCLUDED