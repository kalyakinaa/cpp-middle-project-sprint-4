#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric_accumulator::metric_accumulator_impl::test {

namespace {

metric::MetricResult MakeResult(int value) { return metric::MetricResult{.metric_name = "test", .value = value}; }

}  // namespace

TEST(SumAverageAccumulator, Empty) {
    SumAverageAccumulator acc;
    acc.Finalize();
    auto result = acc.Get();

    EXPECT_EQ(result.sum, 0);
    EXPECT_DOUBLE_EQ(result.average, 0.0);
}

TEST(SumAverageAccumulator, Single) {
    SumAverageAccumulator acc;
    acc.Accumulate(MakeResult(7));
    acc.Finalize();
    auto result = acc.Get();

    EXPECT_EQ(result.sum, 7);
    EXPECT_DOUBLE_EQ(result.average, 7.0);
}

TEST(SumAverageAccumulator, Multiple) {
    SumAverageAccumulator acc;
    acc.Accumulate(MakeResult(1));
    acc.Accumulate(MakeResult(2));
    acc.Accumulate(MakeResult(3));
    acc.Finalize();
    auto result = acc.Get();

    EXPECT_EQ(result.sum, 6);
    EXPECT_DOUBLE_EQ(result.average, 2.0);
}

TEST(SumAverageAccumulator, Reset) {
    SumAverageAccumulator acc;
    acc.Accumulate(MakeResult(100));
    acc.Finalize();
    acc.Reset();
    acc.Accumulate(MakeResult(5));
    acc.Finalize();
    auto result = acc.Get();

    EXPECT_EQ(result.sum, 5);
    EXPECT_DOUBLE_EQ(result.average, 5.0);
}

TEST(SumAverageAccumulator, GetBeforeFinalizeThrows) {
    SumAverageAccumulator acc;

    EXPECT_THROW(acc.Get(), std::runtime_error);
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl::test
