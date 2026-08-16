#include "./time.hpp"

Time::Time() {
    Reset();
}

void Time::Reset() {
    m_last_frame = std::chrono::high_resolution_clock::now();
    m_delta_time = 0.0;
    m_total_time = 0.0;
}

void Time::Update() {
    auto current_time = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> elapsed = current_time - m_last_frame;
    m_delta_time = elapsed.count();
    m_total_time += m_delta_time;
    
    m_last_frame = current_time;
}

double Time::GetDeltaTime() const { return m_delta_time; }
double Time::GetTime() const { return m_total_time; }