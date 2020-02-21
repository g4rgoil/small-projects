#ifndef FPS_COUNTER_HPP_INCLUDED
#define FPS_COUNTER_HPP_INCLUDED

#include <glm/glm.hpp>

class FpsCounter 
{
public:
    FpsCounter(uint window_width, uint window_height);
    void update();
    int currentFps() const;
    void draw() const;

private:
    double current_fps_;
    double last_update_time_;

    uint window_width_;
    uint window_height_;
    uint shader_program_;
    uint vertex_array_;
    uint vertex_buffer_;

};

#endif  // FPS_COUNTER_HPP_INCLUDED