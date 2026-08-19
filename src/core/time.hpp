#ifndef ___TIME__H__
#define ___TIME__H__

#include <chrono>

class Time {
private:
    std::chrono::high_resolution_clock::time_point m_last_frame;
    double m_delta_time = 0.0;
    double m_total_time = 0.0;

public:
    Time();
    void Reset() noexcept;
    void Update() noexcept;
    double GetDeltaTime() const noexcept;
    double GetFrameTime() const noexcept;
    double GetTime() const noexcept;
};


#endif