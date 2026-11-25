#include "metric_impl/code_lines_count.hpp"

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

TEST(CodeLinesCountMetric, Simple) {
    auto func = LoadSingleFunction("simple.py");
    CodeLinesCountMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 5);
}

TEST(CodeLinesCountMetric, Comments) {
    auto func = LoadSingleFunction("comments.py");
    CodeLinesCountMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 3);
}

TEST(CodeLinesCountMetric, ManyLines) {
    auto func = LoadSingleFunction("many_lines.py");
    CodeLinesCountMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_GT(std::get<int>(result.value), 5);
}

TEST(CodeLinesCountMetric, If) {
    auto func = LoadSingleFunction("if.py");
    CodeLinesCountMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 3);
}

TEST(CodeLinesCountMetric, NestedIf) {
    auto func = LoadSingleFunction("nested_if.py");
    CodeLinesCountMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_GT(std::get<int>(result.value), 0);
}

TEST(CodeLinesCountMetric, Exceptions) {
    auto func = LoadSingleFunction("exceptions.py");
    CodeLinesCountMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 7);
}

TEST(CodeLinesCountMetric, Loops) {
    auto func = LoadSingleFunction("loops.py");
    CodeLinesCountMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 6);
}

TEST(CodeLinesCountMetric, ManyParameters) {
    auto func = LoadSingleFunction("many_parameters.py");
    CodeLinesCountMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 1);
}

TEST(CodeLinesCountMetric, MatchCase) {
    auto func = LoadSingleFunction("match_case.py");
    CodeLinesCountMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 7);
}

TEST(CodeLinesCountMetric, Ternary) {
    auto func = LoadSingleFunction("ternary.py");
    CodeLinesCountMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<int>(result.value), 1);
}

}  // namespace analyzer::metric::metric_impl
