#ifndef FPS_LIMITER_HPP_INCLUDED
#define FPS_LIMITER_HPP_INCLUDED

class FpsLimiter
{
  public:
    FpsLimiter();
    FpsLimiter(float target_fps);
    float target_fps() const;
    void target_fps(float target_fps);
    float shift_target_fps(float offset) noexcept;
    void wait();

  private:
    float frame_time() const;

    float target_fps_;
    float last_frame_;
};

#endif  // FPS_LIMITER_HPP_INCLUDED