#include <gtest/gtest.h>
#include "../../src/core/filter/dc_blocker.h"
#include <math.h>
#include "../frequency_test_utilities.h"

using algae::dsp::core::filter::dc_block_t;
using algae::dsp::core::filter::process;
TEST(DSP_Test, CORE_DC_blocker_blocks_DC_offset) { 
    double accum;
    dc_block_t<double> filter = dc_block_t<double>();

    auto accumulateDC = [&](){
        accum += 100.1;
    };

    for(int i=0; i<1000; i++){
        accumulateDC();
        double input = accum + sin(3.14159*i*2020.0);
        filter = process<double>(filter, input);
    }

    EXPECT_FLOAT_EQ(0, filter.y1);
}

TEST(DSP_Test, CORE_DC_blocker_spectrum){
    const size_t NUM_BINS = 32;
    const double SAMPLE_RATE = 48000;

    dc_block_t<double> filter;
    
    std::array<double, NUM_BINS> amp_response = compute_amplitude_response<double, dc_block_t<double>, NUM_BINS>(filter, SAMPLE_RATE);

    //expected values computed in octave using freqz function to evaluate filter response based on coefficients
    std::array<double, NUM_BINS> expected_response = {
        0,
        1.00120421840615,
        1.00218184851231,
        1.0023632033379,
        1.0024266981061,
        1.00245608822917,
        1.00247205113464,
        1.00248167327251,
        1.00248791504363,
        1.0024921907885,
        1.00249524543483,
        1.00249750159972,
        1.00249921351398,
        1.0025005415413,
        1.00250159087691,
        1.00250243283332,
        1.00250311712382,
        1.00250367924569,
        1.00250414507591,
        1.00250453381994,
        1.0025048599547,
        1.0025051345391,
        1.00250536611637,
        1.00250556134671,
        1.00250572545793,
        1.00250586257083,
        1.00250597593673,
        1.00250606811216,
        1.00250614108804,
        1.00250619638473,
        1.00250623512142,
        1.00250625806529
    };
    for(size_t bin_idx=0; bin_idx<NUM_BINS; bin_idx++){
        EXPECT_NEAR(expected_response[bin_idx], amp_response[bin_idx],0.8);
        // std::cout << amp_response[bin_idx] << std::endl;
    }
    // EXPECT_EQ(1,0);

}