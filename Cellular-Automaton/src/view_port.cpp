#include "view_port.hpp"

ViewPort::ViewPort(glm::vec2 max_size) :
        max_size_(max_size),
        bottom_left_(0.0f, 0.0f),
        top_right_(max_size)
{ }

glm::vec2 ViewPort::bottom_left() const
{
    return bottom_left_;
}

glm::vec2 ViewPort::top_right() const
{
    return top_right_;
}

glm::vec2 ViewPort::max_size() const
{
    return max_size_;
}

glm::vec2 ViewPort::size() const
{
    return top_right_ - bottom_left_;
}

void ViewPort::move(glm::vec2 delta)
{
    glm::vec2 new_bottom_left = glm::max(bottom_left_ + delta, 0.0f);
    glm::vec2 new_top_right = glm::min(max_size_, new_bottom_left + size());

    bottom_left_ = new_top_right - size(), top_right_ = new_top_right;
}

void ViewPort::resize(glm::vec2 delta)
{
    glm::vec2 new_size = glm::min(max_size_, glm::max(size() + 2.0f * delta, 2.0f));
    glm::vec2 new_bottom_left = glm::max(bottom_left_ - 0.5f * (new_size - size()), 0.0f);
    glm::vec2 new_top_right = glm::min(max_size_, new_bottom_left + new_size);

    bottom_left_ = new_top_right - new_size, top_right_ = new_top_right;
}