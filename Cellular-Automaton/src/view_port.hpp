#ifndef VIEW_PORT_HPP_INCLUDED
#define VIEW_PORT_HPP_INCLUDED

#include <glm/glm.hpp>

class ViewPort
{
  public:
    ViewPort(glm::vec2 max_size);
    glm::vec2 bottom_left() const;
    glm::vec2 top_right() const;
    glm::vec2 max_size() const;
    glm::vec2 size() const;
    void move(glm::vec2 delta);
    void resize(glm::vec2 delta);

  private:
    glm::vec2 max_size_;
    glm::vec2 bottom_left_;
    glm::vec2 top_right_;
};

#endif  // VIEW_PORT_HPP_INCLUDED