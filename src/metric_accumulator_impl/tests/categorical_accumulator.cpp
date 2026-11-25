#include "metric_accumulator_impl/categorical_accumulator.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

namespace analyzer::metric_accumulator::metric_accumulator_impl::test {

namespace {

metric::MetricResult MakeResult(std::string value) {
    return metric::MetricResult{.metric_name = "test", .value = std::move(value)};
}

}  // namespace

TEST(CategoricalAccumulator, Empty) {
    CategoricalAccumulator acc;
    acc.Finalize();

    EXPECT_TRUE(acc.Get().empty());
}

TEST(CategoricalAccumulator, Single) {
    CategoricalAccumulator acc;
    acc.Accumulate(MakeResult("Snake Case"));
    acc.Finalize();

    EXPECT_EQ(acc.Get().at("Snake Case"), 1);
}

TEST(CategoricalAccumulator, MultipleSame) {
    CategoricalAccumulator acc;
    acc.Accumulate(MakeResult("Pascal Case"));
    acc.Accumulate(MakeResult("Pascal Case"));
    acc.Finalize();

    EXPECT_EQ(acc.Get().at("Pascal Case"), 2);
}

TEST(CategoricalAccumulator, MultipleDifferent) {
    CategoricalAccumulator acc;
    acc.Accumulate(MakeResult("Snake Case"));
    acc.Accumulate(MakeResult("Camel Case"));
    acc.Accumulate(MakeResult("Snake Case"));
    acc.Finalize();

    EXPECT_EQ(acc.Get().at("Snake Case"), 2);
    EXPECT_EQ(acc.Get().at("Camel Case"), 1);
}

TEST(CategoricalAccumulator, Reset) {
    CategoricalAccumulator acc;
    acc.Accumulate(MakeResult("A"));
    acc.Finalize();
    acc.Reset();
    acc.Accumulate(MakeResult("B"));
    acc.Finalize();

    EXPECT_EQ(acc.Get().count("A"), 0);
    EXPECT_EQ(acc.Get().at("B"), 1);
}

TEST(CategoricalAccumulator, GetBeforeFinalizeThrows) {
    CategoricalAccumulator acc;

    EXPECT_THROW(acc.Get(), std::runtime_error);
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl::test
