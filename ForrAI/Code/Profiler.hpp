#pragma once
#include <iostream>
#include <chrono>

namespace fa {
    class TimeScope {
    public:
        TimeScope(const std::string& label) noexcept {
            m_Label = label;
            m_Start = std::chrono::high_resolution_clock::now();
        }
        ~TimeScope() noexcept {
            time_point end        = std::chrono::high_resolution_clock::now();
            double     elapsed_ms = std::chrono::duration<double, std::milli>(end - m_Start).count();
            std::cerr << "[PROFILER]." << m_Label << " : " << elapsed_ms << " ms" << std::endl;
        }

        void stop() noexcept { this->~TimeScope(); }

    private:
        using time_point = std::chrono::high_resolution_clock::time_point;
        time_point  m_Start{};
        std::string m_Label{};
    };

#define FA_CONCATENATE_DIRECT(x, y) x##y
#define FA_CONCATENATE(x, y) FA_CONCATENATE_DIRECT(x, y)
#define FA_VARIABLE(str) FA_CONCATENATE(str, __LINE__)

#define FA_SCOPE_TIMER(label) fa::TimeScope FA_VARIABLE(scope_, __LINE__)(std::string(label));
} // namespace fa
