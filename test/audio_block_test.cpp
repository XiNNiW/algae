#include <gtest/gtest.h>
#include "../src/core/audio_block.h"

using algae::dsp::core::AudioBlock;


TEST(DSP_Test, Core_block_multiply) { 
    
   constexpr auto SR = 48000.0;
   constexpr auto BLOCKSIZE = 64;

   AudioBlock<float,BLOCKSIZE> x;
   AudioBlock<float,BLOCKSIZE> y;

   for(size_t idx;idx<BLOCKSIZE;idx++){
       x[idx] = 2;
       y[idx] = 3;
   }

    AudioBlock<float,BLOCKSIZE> output = x*y;


   for(size_t idx;idx<BLOCKSIZE;idx++){
       EXPECT_FLOAT_EQ(6, output[idx]);
   }

   output *= x;

   for(size_t idx;idx<BLOCKSIZE;idx++){
       EXPECT_FLOAT_EQ(12, output[idx]);
   }

   output = x * 3;

   for(size_t idx;idx<BLOCKSIZE;idx++){
       EXPECT_FLOAT_EQ(6, output[idx]);
   }
   
    output *= 2;

   for(size_t idx;idx<BLOCKSIZE;idx++){
       EXPECT_FLOAT_EQ(12, output[idx]);
   }

}

TEST(DSP_Test, Core_block_divide) { 
    
   constexpr auto SR = 48000.0;
   constexpr auto BLOCKSIZE = 64;

   AudioBlock<float,BLOCKSIZE> x;
   AudioBlock<float,BLOCKSIZE> y;

   for(size_t idx;idx<BLOCKSIZE;idx++){
       x[idx] = 9;
       y[idx] = 3;
   }

    AudioBlock<float,BLOCKSIZE> output = x/y;


   for(size_t idx;idx<BLOCKSIZE;idx++){
       EXPECT_FLOAT_EQ(3, output[idx]);
   }

   output /= y;

   for(size_t idx;idx<BLOCKSIZE;idx++){
       EXPECT_FLOAT_EQ(1, output[idx]);
   }


}

TEST(DSP_Test, Core_block_add) { 
    
   constexpr auto SR = 48000.0;
   constexpr auto BLOCKSIZE = 64;

   AudioBlock<float,BLOCKSIZE> x;
   AudioBlock<float,BLOCKSIZE> y;

   for(size_t idx;idx<BLOCKSIZE;idx++){
       x[idx] = 9;
       y[idx] = 3;
   }

    AudioBlock<float,BLOCKSIZE> output = x+y;


   for(size_t idx;idx<BLOCKSIZE;idx++){
       EXPECT_FLOAT_EQ(12, output[idx]);
   }

   output += y;

   for(size_t idx;idx<BLOCKSIZE;idx++){
       EXPECT_FLOAT_EQ(15, output[idx]);
   }


}

TEST(DSP_Test, Core_block_subtract) { 
    
   constexpr auto SR = 48000.0;
   constexpr auto BLOCKSIZE = 64;

   AudioBlock<float,BLOCKSIZE> x;
   AudioBlock<float,BLOCKSIZE> y;

   for(size_t idx;idx<BLOCKSIZE;idx++){
       x[idx] = 9;
       y[idx] = 3;
   }

    AudioBlock<float,BLOCKSIZE> output = x-y;


   for(size_t idx;idx<BLOCKSIZE;idx++){
       EXPECT_FLOAT_EQ(6, output[idx]);
   }

   output -= y;

   for(size_t idx;idx<BLOCKSIZE;idx++){
       EXPECT_FLOAT_EQ(3, output[idx]);
   }


}