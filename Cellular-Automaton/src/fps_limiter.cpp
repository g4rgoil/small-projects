#include "fps_limiter.hpp"

#include <stdexcept>
#include <GLFW/glfw3.h>


FpsLimiter::FpsLimiter() : target_fps_(0.0f)
{ }

FpsLimiter::FpsLimiter(float target_fps) : target_fps_(target_fps)
{
    if (target_fps_ < 0.0f) {
        throw std::invalid_argument("Target fps must not be smaller than 0.");
    }
}

float FpsLimiter::target_fps() const
{
    return target_fps_;
}

void FpsLimiter::target_fps(float target_fps)
{
    if (target_fps < 0.0f) {
        throw std::invalid_argument("Target fps must not be smaller than 0.");
    }

    target_fps_ = target_fps;
}

float FpsLimiter::shift_target_fps(float offset) noexcept
{
    return target_fps_ = std::max(target_fps_ + offset, 1.0f);
}

void FpsLimiter::wait()
{
    if (target_fps_ > 0.0f) {
        float now, next_frame = last_frame_ + frame_time();

        do {
            // TODO get rid of busy waiting ?!
            now = (float)glfwGetTime() * 1000.0f;
        } while (now < next_frame);

        last_frame_ = next_frame;
    }
}

float FpsLimiter::frame_time() const
{
    return (target_fps_ > 0.0f) ? 1000.0f / target_fps_ : 0.0f;
}