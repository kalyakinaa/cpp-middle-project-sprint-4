#include "metric_impl/naming_style.hpp"

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

TEST(NamingStyleMetric, SimpleSnake) {
    auto func = LoadSingleFunction("simple.py");
    NamingStyleMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<std::string>(result.value), "Snake Case");
}

TEST(NamingStyleMetric, SnakeWithUpper) {
    auto func = LoadSingleFunction("comments.py");
    NamingStyleMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<std::string>(result.value), "Unknown");
}

TEST(NamingStyleMetric, PascalCase) {
    auto func = LoadSingleFunction("exceptions.py");
    NamingStyleMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<std::string>(result.value), "Unknown");
}

TEST(NamingStyleMetric, CamelCase) {
    auto func = LoadSingleFunction("if.py");
    NamingStyleMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<std::string>(result.value), "Camel Case");
}

TEST(NamingStyleMetric, PascalTestLoops) {
    auto func = LoadSingleFunction("loops.py");
    NamingStyleMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<std::string>(result.value), "Pascal Case");
}

TEST(NamingStyleMetric, LowerCase) {
    auto func = LoadSingleFunction("many_lines.py");
    NamingStyleMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<std::string>(result.value), "Lower Case");
}

TEST(NamingStyleMetric, MatchCase) {
    auto func = LoadSingleFunction("match_case.py");
    NamingStyleMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<std::string>(result.value), "Unknown");
}

TEST(NamingStyleMetric, NestedIf) {
    auto func = LoadSingleFunction("nested_if.py");
    NamingStyleMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<std::string>(result.value), "Pascal Case");
}

TEST(NamingStyleMetric, ManyParameters) {
    auto func = LoadSingleFunction("many_parameters.py");
    NamingStyleMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<std::string>(result.value), "Snake Case");
}

TEST(NamingStyleMetric, Ternary) {
    auto func = LoadSingleFunction("ternary.py");
    NamingStyleMetric metric;
    auto result = metric.Calculate(func);

    EXPECT_EQ(std::get<std::string>(result.value), "Unknown");
}

}  // namespace analyzer::metric::metric_impl
