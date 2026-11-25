#include "metric_accumulator_impl/average_accumulator.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric_accumulator::metric_accumulator_impl::test {

namespace {

metric::MetricResult MakeResult(int value) { return metric::MetricResult{.metric_name = "test", .value = value}; }

}  // namespace

TEST(AverageAccumulator, Empty) {
    AverageAccumulator acc;
    acc.Finalize();

    EXPECT_DOUBLE_EQ(acc.Get(), 0.0);
}

TEST(AverageAccumulator, Single) {
    AverageAccumulator acc;
    acc.Accumulate(MakeResult(5));
    acc.Finalize();

    EXPECT_DOUBLE_EQ(acc.Get(), 5.0);
}

TEST(AverageAccumulator, Multiple) {
    AverageAccumulator acc;
    acc.Accumulate(MakeResult(2));
    acc.Accumulate(MakeResult(4));
    acc.Accumulate(MakeResult(6));
    acc.Finalize();

    EXPECT_DOUBLE_EQ(acc.Get(), 4.0);
}

TEST(AverageAccumulator, Reset) {
    AverageAccumulator acc;
    acc.Accumulate(MakeResult(10));
    acc.Finalize();
    acc.Reset();
    acc.Accumulate(MakeResult(3));
    acc.Finalize();

    EXPECT_DOUBLE_EQ(acc.Get(), 3.0);
}

TEST(AverageAccumulator, GetBeforeFinalizeThrows) {
    AverageAccumulator acc;
    acc.Accumulate(MakeResult(1));

    EXPECT_THROW(acc.Get(), std::runtime_error);
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl::test
