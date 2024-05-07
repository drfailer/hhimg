#ifndef PERF_RECORDER_HPP
#define PERF_RECORDER_HPP
#include <chrono>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

namespace hhimg::utils {

class PerfRectorder {
  public:
    PerfRectorder() = delete;

    static void start(std::string const &desc) {
        timers_.insert({desc, std::chrono::system_clock::now()});
        if (!durations_.contains(desc)) {
            durations_.insert({desc, {}});
        }
    }

    static void end(std::string const &desc) {
        auto end = std::chrono::system_clock::now();
        durations_.at(desc).push_back(end - timers_.at(desc));
        timers_.erase(desc);
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
    static inline std::map<std::string, std::chrono::time_point<std::chrono::system_clock>>
        timers_;
    static inline std::map<std::string,
                           std::vector<std::chrono::duration<double>>>
        durations_ = {};

    static void report(std::string const &desc,
                       std::vector<std::chrono::duration<double>> times) {
        double avg = avgTime(times);
        double stddev = stdDev(times, avg);
        double min = minTime(times);
        double max = maxTime(times);
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

    static double maxTime(std::vector<std::chrono::duration<double>> times) {
        double max = times.at(0).count();

        for (auto time : times) {
            if (time.count() > max) {
                max = time.count();
            }
        }
        return max;
    }

    static double minTime(std::vector<std::chrono::duration<double>> times) {
        double min = times.at(0).count();

        for (auto time : times) {
            if (time.count() < min) {
                min = time.count();
            }
        }
        return min;
    }
};

} // namespace hhimg::utils

#endif
