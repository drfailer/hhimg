#ifndef PERF_RECORDER_HPP
#define PERF_RECORDER_HPP
#include <chrono>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <stack>
#include <vector>

namespace hhimg::utils {

class PerfRectorder {
  public:
    PerfRectorder() = delete;

    static void start(std::string const &desc) {
        timers_.push(std::chrono::system_clock::now());
        if (!durations_.contains(desc)) {
            durations_.insert({desc, {}});
        }
    }

    static void end(std::string const &desc) {
        auto end = std::chrono::system_clock::now();
        durations_.at(desc).push_back(end - timers_.top());
        timers_.pop();
    }

    static void report() {
        for (auto elt : durations_) {
            if (elt.second.size() > 1) {
                report(elt.first, elt.second);
            } else {
                std::cout << elt.first << ": " << elt.second.at(0)
                          << std::endl;
            }
        }
    }

  private:
    static inline std::stack<std::chrono::time_point<std::chrono::system_clock>>
        timers_;
    static inline std::map<std::string,
                           std::vector<std::chrono::duration<double>>>
        durations_ = {};

    static void report(std::string const &desc,
                       std::vector<std::chrono::duration<double>> times) {
        double avg = avgTime(times);
        double stddev = stdDev(times, avg);
        double min = std::min(times.begin(), times.end())->count();
        double max = std::max(times.begin(), times.end())->count();
        std::cout << desc << ": " << avg << "s +- " << stddev
                  << "s (min: " << min << "s, " << max << "s)" << std::endl;
    }

    static double avgTime(std::vector<std::chrono::duration<double>> times) {
        double avg = std::accumulate(times.begin(), times.end(),
                                     std::chrono::duration<double>().zero())
                         .count() /
                     times.size();
        return avg;
    }

    static double stdDev(std::vector<std::chrono::duration<double>> times,
                         double avg) {
        double result = 0;
        std::for_each(times.begin(), times.end(), [&](auto elt) {
            auto diff = elt.count() - avg;
            result += diff * diff;
        });
        return std::sqrt(result / times.size());
    }
};

} // namespace hhimg::utils

#endif
