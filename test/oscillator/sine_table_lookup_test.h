#pragma once
#include <gtest/gtest.h>
#include <array>
#include <tuple>
#include <cstddef>

const size_t FREQ_BINS = 4096;
const double SAMPLE_RATE = 48000;
const double resolution = SAMPLE_RATE/(2*FREQ_BINS);
class SineOscFixture :public ::testing::TestWithParam<double> {};
const double inf = std::numeric_limits<double>::infinity();
