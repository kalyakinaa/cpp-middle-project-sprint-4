#include "metric_impl/cyclomatic_complexity.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyzer::metric::metric_impl {

std::string CyclomaticComplexityMetric::Name() const { return kName; }

MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function &f) const {
    auto &function_ast = f.ast;

    constexpr std::array<std::string_view, 8> complexity_nodes = {
        "if_statement",            // if
        "elif_clause",             // elif
        "for_statement",           // for
        "while_statement",         // while
        "except_clause",           // except
        "case_clause",             // case
        "assert",                  // assert
        "conditional_expression",  // для тернарного оператора
    };

    auto count_occurrences = [&](std::string_view node_type) {
        auto occurrences = std::views::all(function_ast) | std::views::split(node_type) | std::views::drop(1);
        return static_cast<int>(std::ranges::distance(occurrences));
    };

    int complexity =
        1 + std::accumulate(complexity_nodes.begin(), complexity_nodes.end(), 0,
                            [&](int acc, std::string_view node_type) { return acc + count_occurrences(node_type); });

    return complexity;
}

}  // namespace analyzer::metric::metric_impl
