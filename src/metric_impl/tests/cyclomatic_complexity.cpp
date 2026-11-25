#include "metric_impl/cyclomatic_complexity.hpp"

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

TEST(CyclomaticComplexityMetric, Simple) {
    auto func = LoadSingleFunction("simple.py");
    CyclomaticComplexityMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 2);
}

TEST(CyclomaticComplexityMetric, If) {
    auto func = LoadSingleFunction("if.py");
    CyclomaticComplexityMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 2);
}

TEST(CyclomaticComplexityMetric, NestedIf) {
    auto func = LoadSingleFunction("nested_if.py");
    CyclomaticComplexityMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 5);
}

TEST(CyclomaticComplexityMetric, Loops) {
    auto func = LoadSingleFunction("loops.py");
    CyclomaticComplexityMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 4);
}

TEST(CyclomaticComplexityMetric, Exceptions) {
    auto func = LoadSingleFunction("exceptions.py");
    CyclomaticComplexityMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 3);
}

TEST(CyclomaticComplexityMetric, MatchCase) {
    auto func = LoadSingleFunction("match_case.py");
    CyclomaticComplexityMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 4);
}

TEST(CyclomaticComplexityMetric, Ternary) {
    auto func = LoadSingleFunction("ternary.py");
    CyclomaticComplexityMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 3);
}

TEST(CyclomaticComplexityMetric, ManyLines) {
    auto func = LoadSingleFunction("many_lines.py");
    CyclomaticComplexityMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 2);
}

TEST(CyclomaticComplexityMetric, ManyParameters) {
    auto func = LoadSingleFunction("many_parameters.py");
    CyclomaticComplexityMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 2);
}

TEST(CyclomaticComplexityMetric, Comments) {
    auto func = LoadSingleFunction("comments.py");
    CyclomaticComplexityMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 1);
}

}  // namespace analyzer::metric::metric_impl
