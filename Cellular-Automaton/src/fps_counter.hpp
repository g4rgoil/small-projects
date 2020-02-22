#ifndef FPS_COUNTER_HPP_INCLUDED
#define FPS_COUNTER_HPP_INCLUDED

#include <glm/glm.hpp>
#include <glad/glad.h>

class FpsCounter 
{
  public:
    FpsCounter(uint window_width, uint window_height);
    FpsCounter(const FpsCounter&) = delete;
    FpsCounter(FpsCounter&&) = default;
    FpsCounter& operator=(const FpsCounter&) = delete;
    FpsCounter& operator=(FpsCounter&&) = default;
    ~FpsCounter();

    void update();
    int currentFps() const;
    void draw() const;

  private:
    double current_fps_;
    double last_update_time_;

    uint window_width_;
    uint window_height_;
    GLuint shader_program_;
    GLuint vertex_array_;
    GLuint vertex_buffer_;

};

#endif  // FPS_COUNTER_HPP_INCLUDED