#include "metric_impl/parameters_count.hpp"

#include <gtest/gtest.h>

#include "file.hpp"
#include "function.hpp"

namespace analyzer::metric::metric_impl {

namespace {

function::Function LoadSingleFunction(const std::string &filename) {
    file::File f{filename};
    function::FunctionExtractor extractor;
    auto funcs = extractor.Get(f);
    if (funcs.empty()) {
        throw std::runtime_error("No functions found in " + filename);
    }
    return funcs.front();
}

}  // namespace

TEST(CountParametersMetric, Simple) {
    auto func = LoadSingleFunction("simple.py");
    CountParametersMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 0);
}

TEST(CountParametersMetric, Comments) {
    auto func = LoadSingleFunction("comments.py");
    CountParametersMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 3);
}

TEST(CountParametersMetric, ManyParameters) {
    auto func = LoadSingleFunction("many_parameters.py");
    CountParametersMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_GE(std::get<int>(result.value), 5);
}

TEST(CountParametersMetric, If) {
    auto func = LoadSingleFunction("if.py");
    CountParametersMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 1);
}

TEST(CountParametersMetric, NestedIf) {
    auto func = LoadSingleFunction("nested_if.py");
    CountParametersMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 2);
}

TEST(CountParametersMetric, Loops) {
    auto func = LoadSingleFunction("loops.py");
    CountParametersMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 1);
}

TEST(CountParametersMetric, Exceptions) {
    auto func = LoadSingleFunction("exceptions.py");
    CountParametersMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 0);
}

TEST(CountParametersMetric, ManyLines) {
    auto func = LoadSingleFunction("many_lines.py");
    CountParametersMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 0);
}

TEST(CountParametersMetric, MatchCase) {
    auto func = LoadSingleFunction("match_case.py");
    CountParametersMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 1);
}

TEST(CountParametersMetric, Ternary) {
    auto func = LoadSingleFunction("ternary.py");
    CountParametersMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 1);
}

}  // namespace analyzer::metric::metric_impl
