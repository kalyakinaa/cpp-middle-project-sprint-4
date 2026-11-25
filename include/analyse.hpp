#pragma once

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"

namespace analyzer {

namespace rv = std::ranges::views;
namespace rs = std::ranges;

auto AnalyseFunctions(const std::vector<std::string> &files,
                      const analyzer::metric::MetricExtractor &metric_extractor) {
    auto per_file_functions = files | rv::transform([](const std::string &filename) {
                                  file::File f{filename};
                                  function::FunctionExtractor extractor;
                                  return extractor.Get(f);
                              }) |
                              rv::join;

    return per_file_functions | rv::transform([&metric_extractor](function::Function func) {
               auto metrics = metric_extractor.Get(func);
               return std::pair{std::move(func), std::move(metrics)};
           }) |
           rs::to<std::vector<decltype(std::pair{std::declval<function::Function>(),
                                                 std::declval<metric::MetricResults>()})>>();
}

auto SplitByClasses(const auto &analysis) {
    auto class_methods = analysis | rv::filter([](const auto &elem) { return elem.first.class_name.has_value(); });

    return class_methods |
           rv::chunk_by([](const auto &lhs, const auto &rhs) { return lhs.first.class_name == rhs.first.class_name; });
}

auto SplitByFiles(const auto &analysis) {
    return analysis |
           rv::chunk_by([](const auto &lhs, const auto &rhs) { return lhs.first.filename == rhs.first.filename; });
}

void AccumulateFunctionAnalysis(const auto &analysis,
                                const analyzer::metric_accumulator::MetricsAccumulator &accumulator) {
    rs::for_each(analysis,
                 [&accumulator](const auto &elem) { accumulator.AccumulateNextFunctionResults(elem.second); });
}

}  // namespace analyzer
