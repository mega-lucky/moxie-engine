#include "./time.hpp"

Time::Time() {
    Reset();
}

void Time::Reset() noexcept {
    m_last_frame = std::chrono::high_resolution_clock::now();
    m_delta_time = 0.0;
    m_total_time = 0.0;
}

void Time::Update() noexcept {
    auto current_time = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> elapsed = current_time - m_last_frame;
    m_delta_time = elapsed.count();
    m_total_time += m_delta_time;
    
    m_last_frame = current_time;
}

double Time::GetDeltaTime() const noexcept {
    return m_delta_time;
}
double Time::GetFrameTime() const noexcept {
    return m_delta_time == 0.0
        ? 0.0
        : 1.0/m_delta_time;
}
double Time::GetTime() const noexcept {
    return m_total_time;
}