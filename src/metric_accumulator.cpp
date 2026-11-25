#include "metric_accumulator.hpp"

#include <unistd.h>

#include <algorithm>
#include <any>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyzer::metric_accumulator {

void MetricsAccumulator::AccumulateNextFunctionResults(const std::vector<metric::MetricResult> &metric_results) const {
    rs::for_each(metric_results, [this](const auto &metric_result) {
        auto it = accumulators.find(metric_result.metric_name);
        if (it == accumulators.end()) {
            throw std::runtime_error("No accumulator registered for metric '" + metric_result.metric_name + "'");
        }
        it->second->Accumulate(metric_result);
    });
}

void MetricsAccumulator::ResetAccumulators() {
    rs::for_each(accumulators, [](auto &pair) { pair.second->Reset(); });
}

}  // namespace analyzer::metric_accumulator
