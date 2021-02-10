#include <gtest/gtest.h>
#include "../src/shell/dsp_graph.h"
#include "../src/shell/math_functions.h"
#include <iostream>

using algae::dsp::shell::dsp_node;
using algae::dsp::shell::connect;
using algae::dsp::shell::math::constant_node;

TEST(DSP_Test, ConstantNode) { 
    auto number = constant_node<double,double>(5);

    ASSERT_EQ(5,number.getOutputValue(0));

    number.update();

    ASSERT_EQ(5,number.getOutputValue(0));

}

