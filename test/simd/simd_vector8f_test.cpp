#include "../../src/core/simd/simd_config.h"
#if SSE_INSTR_SET>6
#include <gtest/gtest.h>

#include "../../src/core/simd.h"
#include <cmath>

using algae::dsp::core::simd::vector8f;
using algae::dsp::core::simd::vector8fb;

TEST(SIMD_Test, vector8f_add) {
    float a[8]         = {13.2 , 15.7, 19.5, 1.5, 23.34, 23.1, 54.5, 88882.1};
    float b[8]         = {1.11 , 21.5, 71.9, 9.13, 13.2, 15.7, 19.5, 1.5    };
    float expected[8]  = {14.31, 37.2, 91.40, 10.63, 36.54, 38.8, 74.0, 88883.6};
    float actual[8];
    vector8f av, bv, cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av + bv;
    cv.store_a(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_NEAR(expected[idx], actual[idx], 0.0001);

}
TEST(SIMD_Test, vector8f_add_left_scalar) {
    float a[8]         = {13.2 , 15.7, 19.5, 1.5, 23.34, 23.1, 54.5, 88882.1};
    float expected[8]  = {14.31,	16.81,	20.61,	2.61,	24.45,	24.21,	55.61,	88883.21};
    float actual[8];
    vector8f av, bv, cv;
    av.load_a(a);
    cv = av + vector8f(1.11);
    cv.store_a(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_NEAR(expected[idx], actual[idx], 0.0001);
}
TEST(SIMD_Test, vector8f_add_right_scalar) {
    float a[8]         = {13.2 , 15.7, 19.5, 1.5, 23.34, 23.1, 54.5, 88882.1};
    float expected[8]  = {14.31,	16.81,	20.61,	2.61,	24.45,	24.21,	55.61,	88883.21};
    float actual[8];
    vector8f av, cv;
    av.load_a(a);
    cv = vector8f(1.11) + av;
    cv.store_a(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_NEAR(expected[idx], actual[idx], 0.0001);

}

TEST(SIMD_Test, vector8f_sub) {
    float a[8]         = {13.2 , 15.7, 19.5, 1.5, 23.34, 23.1, 54.5, 88882.1};
    float b[8]         = {1.11 , 21.5, 71.9, 9.13, 13.2, 15.7, 19.5, 1.5    };
    float expected[8]  = {12.09,-5.8,-52.4,-7.63,10.14,	7.4,	35.0,	88880.6};
    float actual[8];
    vector8f av, bv, cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av - bv;
    cv.store_a(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector8f_sub_left_scalar) {
    float a[8]         = {13.2 , 15.7, 19.5, 1.5, 23.34, 23.1, 54.5, 88882.1};
    float expected[8]  = {12.09,	14.59,	18.39,	0.39,	22.23,	21.99,	53.39,	88880.99,};
    float actual[8];
    vector8f av, cv;
    av.load_a(a);
    cv = av - vector8f(1.11);
    cv.store_a(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector8f_sub_right_scalar) {
    float a[8]         = {13.2 , 15.7, 19.5, 1.5, 23.34, 23.1, 54.5, 88882.1};
    float expected[8]  = {-12.09,	-14.59,	-18.39,	-0.39,	-22.23,	-21.99,	-53.39,	-88880.99};
    float actual[8];
    vector8f av, cv;
    av.load_a(a);
    cv = vector8f(1.11) - av;
    cv.store_a(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);

}

TEST(SIMD_Test, vector8f_mul) {
    float a[8]         = {13.2 , 15.7, 19.5, 1.5, 23.34, 23.1, 54.5, 88882.1};
    float b[8]         = {1.11 , 21.5, 71.9, 9.13, 13.2, 15.7, 19.5, 1.5    };
    float expected[8]  = {14.652, 337.55,1402.05,13.695,308.088,362.67,1062.75,133323.15};
    float actual[8];
    vector8f av, bv, cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av * bv;
    cv.store_a(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector8f_mul_left_scalar) {
    float a[8]         = {13.2 , 15.7, 19.5, 1.5, 23.34, 23.1, 54.5, 88882.1};
    float expected[8]  = {14.652,17.427,21.645,1.665,25.9074,25.641,60.495,98659.131};
    float actual[8];
    vector8f av, cv;
    av.load_a(a);
    cv = av * vector8f(1.11);
    cv.store_a(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector8f_mul_right_scalar) {
    float a[8]         = {13.2 , 15.7, 19.5, 1.5, 23.34, 23.1, 54.5, 88882.1};
    float expected[8]  = {14.652,17.427,21.645,1.665,25.9074,25.641,60.495,98659.131};
    float actual[8];
    vector8f av, cv;
    av.load_a(a);
    cv = vector8f(1.11) * av;
    cv.store_a(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);

}

TEST(SIMD_Test, vector8f_div) {
    float a[8]         = {13.2 , 15.7, 19.5, 1.5, 23.34, 23.1, 54.5, 88882.1};
    float b[8]         = {1.11 , 21.5, 71.9, 9.13, 13.2, 15.7, 19.5, 1.5    };
    float expected[8]  = {11.89189189189190000,0.73023255813953500,0.27121001390820600,0.16429353778751400,1.76818181818182000,1.47133757961783000,2.79487179487179000,59254.73333333330000000};
    float actual[8];
    vector8f av, bv, cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av / bv;
    cv.store_a(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector8f_div_left_scalar) {
    float a[8]         = {13.2 , 15.7, 19.5, 1.5, 23.34, 23.1, 54.5, 88882.1};
    float expected[8]  = {11.89189189189190000,14.14414414414410000,17.56756756756760000,1.35135135135135000,21.02702702702700000,20.81081081081080000,49.09909909909910000,80073.96396396400000000};
    float actual[8];
    vector8f av, cv;
    av.load_a(a);
    cv = av / vector8f(1.11);
    cv.store_a(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector8f_div_right_scalar) {
    float a[8]         = {13.2 , 15.7, 19.5, 1.5, 23.34, 23.1, 54.5, 88882.1};
    float expected[8]  = {0.08409090909090910,0.07070063694267520,0.05692307692307690,0.74000000000000000,0.04755784061696660,0.04805194805194810,0.02036697247706420,0.00001248845380566};
    float actual[8];
    vector8f av, cv;
    av.load_a(a);
    cv = vector8f(1.11) / av;
    cv.store_a(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);

}

TEST(SIMD_Test, vector8f_div_0) {
    float a[8]         = {13.2 , 15.7, 19.5, 1.5, 23.34, 23.1, 54.5, 88882.1};
    float b[8]         = {1.11 , 0, 71.9, 9.13, 13.2, 15.7, 19.5, 1.5    };
    float expected[8]  = {11.89189189189190000,0,0.27121001390820600,0.16429353778751400,1.76818181818182000,1.47133757961783000,2.79487179487179000,59254.73333333330000000};
    float actual[8];
    vector8f av, bv, cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av / bv;
    cv.store_a(actual);
    
    EXPECT_NEAR(expected[0],actual[0],0.0001);
    EXPECT_TRUE(std::isinf(actual[1]));
    EXPECT_NEAR(expected[2],actual[2],0.0001);
    EXPECT_NEAR(expected[3],actual[3],0.0001);
    EXPECT_NEAR(expected[4],actual[4],0.0001);
    EXPECT_NEAR(expected[5],actual[5],0.0001);
    EXPECT_NEAR(expected[6],actual[6],0.0001);
    EXPECT_NEAR(expected[7],actual[7],0.0001);

}

// //comparison

TEST(SIMD_Test, vector8f_gt) {
    float a[8]         = {13.2 , 15.7, 19.5, 1.5, 23.34, 23.1, 54.5, 88882.1};
    float b[8]         = {1.11 , 21.5, 71.9, 9.13, 13.2, 15.7, 19.5, 1.5    };
    bool expected[8]   = {true, false, false, false, true, true, true, true};
    bool actual[8];
    vector8f av, bv;
    vector8fb cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av > bv;
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector8f_gt_right_scalar) {
    float a[8]         = {0.5 , 15.7, 19.5, 1.5, 23.34, 23.1, -44.2, 88882.1};
    bool expected[8]  = {false, true, true, true, true, true, false, true};
    bool actual[8];
    vector8f av, bv;
    vector8fb cv;
    av.load_a(a);
    cv = av > vector8f(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector8f_gt_left_scalar) {
    float a[8]         = {0.5 , 15.7, 19.5,  1.11, 23.34, 23.1, -44.2, 88882.1};
    bool expected[8]  = {true, false, false, false, false, false, true, false};
    bool actual[8];
    vector8f av;
    vector8fb cv;
    av.load_a(a);
    cv = vector8f(1.11) > av;
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector8f_gte) {
    float a[8]         = {13.2 , 15.7, 71.9, 1.5, 23.34, 23.1, 19.5, 88882.1};
    float b[8]         = {1.11 , 21.5, 71.9, 9.13, 13.2, 15.7, 19.5, 1.5    };
    bool expected[8]  =  {true, false, true, false, true, true, true, true};
    bool actual[8];
    vector8f av, bv;
    vector8fb cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av >= bv;
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector8f_gte_right_scalar) {
    float a[8]         = {0.5 , 15.7, 71.9, 1.5, 23.34, -4, 19.5, 88882.1};
    bool expected[8]   = {false, true, true, true,true, false, true, true};
    bool actual[8];
    vector8f av, bv;
    vector8fb cv;
    av.load_a(a);
    cv = av >= vector8f(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector8f_gte_left_scalar) {
    float a[8]         = {1.11 , -3, 71.9, 1.5, 23.34, -4, 19.5, 88882.1};
    bool expected[8]   = {true, true, false, false, false, true, false, false};
    bool actual[8];
    vector8f av;
    vector8fb cv;
    av.load_a(a);
    cv = vector8f(1.11) >= av;
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector8f_lt) {
    float a[8]         = {13.2 , 15.7, 19.5, 1.5, 23.34, 23.1, 54.5, 88882.1};
    float b[8]         = {1.11 , 21.5, 71.9, 9.13, 13.2, 15.7, 19.5, 1.5    };
    bool expected[8]   = {false, true, true, true, false, false, false, false};
    bool actual[8];
    vector8f av, bv;
    vector8fb cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av < bv;
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector8f_lt_right_scalar) {
    float a[8]         = {0.5, 15.7, 19.5, 1.5, 1.11, -4, 54.5, 88882.1};
    bool expected[8]   = {true, false, false, false, false, true, false, false};
    bool actual[8];
    vector8f av, bv;
    vector8fb cv;
    av.load_a(a);
    cv = av < vector8f(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector8f_lt_left_scalar) {
    float a[8]         = {0.5,   15.7, 19.5, 1.5,  1.11, - 4,    54.5, 88882.1};
    bool expected[8]   = {false, true, true, true, false, false, true, true};
    bool actual[8];
    vector8f av;
    vector8fb cv;
    av.load_a(a);
    cv = vector8f(1.11) < av;
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector8f_lte) {
    float a[8]         = {13.2 , 15.7, 19.5, 1.5, 23.34, 23.1, 54.5, 88882.1};
    float b[8]         = {1.11 , 21.5, 19.5, 9.13, 13.2, 15.7, 19.5, 1.5    };
    bool expected[8]   = {false, true, true, true, false, false, false, false};
    bool actual[8];
    vector8f av, bv;
    vector8fb cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av <= bv;
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector8f_lte_right_scalar) {
    float a[8]         = {0.5, 15.7, 19.5, 1.5, 1.11, -4, 54.5, 88882.1};
    bool expected[8]   = {true, false, false, false, true, true, false, false};
    bool actual[8];
    vector8f av, bv;
    vector8fb cv;
    av.load_a(a);
    cv = av <= vector8f(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector8f_lte_left_scalar) {
    float a[8]         = {0.5,   15.7, 19.5, 1.5,  1.11,  -4,    54.5,  88882.1};
    bool expected[8]   = {false, true, true, true, true, false, true, true};
    bool actual[8];
    vector8f av;
    vector8fb cv;
    av.load_a(a);
    cv = vector8f(1.11) <= av;
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++){
        EXPECT_EQ(expected[idx], actual[idx]);
    }

}


TEST(SIMD_Test, vector8f_eq) {
    float a[8]         = {13.2 , 15.7, 71.9, 1.5  , 13.2 , 15.7, 71.9, 1.5  };
    float b[8]         = {1.11 , 15.7, 71.9, 9.13 , 1.11 , 15.7, 71.9, 9.13 };
    bool expected[8]   = {false, true, true, false, false, true, true, false};
    bool actual[8];
    vector8f av, bv;
    vector8fb cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av == bv;
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector8f_eq_right_scalar) {
    float a[8]        =  {0.5 , 1.11, 19.5, 1.11  , 0.5 , 1.11, 19.5, 1.11  };
    bool expected[8]  =  {false, true, false, true, false, true, false, true};
    bool actual[8];
    vector8f av, bv;
    vector8fb cv;
    av.load_a(a);
    cv = av == vector8f(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector8f_eq_left_scalar) {
    float a[8]         = {0.5 , 15.7, 19.5, 1.11  ,0.5 , 15.7, 19.5, 1.11  };
    bool expected[8]  =  {false, false, false, true,false, false, false, true};
    bool actual[8];
    vector8f av;
    vector8fb cv;
    av.load_a(a);
    cv = vector8f(1.11) == av;
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector8f_neq) {
    float a[8]         = {13.2 , 15.7, 71.9, 1.5  ,13.2 , 15.7, 71.9, 1.5  };
    float b[8]         = {1.11 , 15.7, 71.9, 9.13 ,1.11 , 15.7, 71.9, 9.13 };
    bool expected[8]   = {true, false, false, true,true, false, false, true};
    bool actual[8];
    vector8f av, bv;
    vector8fb cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av != bv;
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector8f_neq_right_scalar) {
    float a[8]         = {0.5 , 1.11, 19.5, 1.11  ,0.5 , 1.11, 19.5, 1.11  };
    bool expected[8]  =  {true, false, true, false,true, false, true, false};
    bool actual[8];
    vector8f av, bv;
    vector8fb cv;
    av.load_a(a);
    cv = av != vector8f(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector8f_neq_left_scalar) {
    float a[8]         = {0.5 , 15.7, 19.5, 1.11  ,0.5 , 15.7, 19.5, 1.11  };
    bool expected[8]  =  {true, true, true, false,true, true, true, false};
    bool actual[8];
    vector8f av;
    vector8fb cv;
    av.load_a(a);
    cv = vector8f(1.11) != av;
    cv.store(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

// TRIG

TEST(SIMD_Test, vector8f_sin) {
    float a[8]         = {13.2, 15.7, 19.5, -1.5, 23.34, 23.1, 54.5, -88};
    float expected[8]  = {0.59207351470722300, 0.00796318378593734, 0.60553986971960100, -0.99749498660405500, -0.97547116864901700, -0.89518736781968200, -0.88797583833766300, -0.03539830273366070};
    float actual[8];
    vector8f av;
    vector8f cv;
    av.load_a(a);
    cv = sin(av);
    cv.store_a(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_NEAR(expected[idx], actual[idx],0.0001);

}

TEST(SIMD_Test, vector8f_cos) {
    float a[8]         = {13.2, 15.7, 19.5, -1.5, 23.34, 23.1, 54.5, -88};
    float expected[8]  = {0.80588395764045100, -0.99996829334934000, 0.79581496981394400, 0.07073720166770290, -0.22012723396826800, -0.44569000044433300, -0.45989010701310400, 0.99937328369512500};
    float actual[8];
    vector8f av;
    vector8f cv;
    av.load_a(a);
    cv = cos(av);
    cv.store_a(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_NEAR(expected[idx], actual[idx], 0.0001);

}

TEST(SIMD_Test, vector8f_exp_matches_single_number_version) {
    // significant precision loss for numbers greater than 60 or so....
    float a[8]         = {13.2, 15.7, 1.9, -1.5, 13.2, 15.7, -1.9, 1.5};
    // float expected[8]; // = {540364.93724669200000000, 6582992.58458373000000000, 294267566.04150900000000000, 0.22313016014843000, 13690938122.32320000000000000, 10769673371.15770000000000000, 466712318321364000000000.00000000000000000, 0.00000000000000000};
    float actual[8];
    vector8f av;
    vector8f cv;
    av.load_a(a);
    cv = exp(av);
    cv.store_a(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_NEAR(exp(a[idx]), actual[idx],0.0001);

}

TEST(SIMD_Test, vector8f_log) {
    float a[8]         = {13.2, 15.7, 19.5, 1.5, 23.34, 23.1, 54.5, 88};
    // float expected[8]  = {1.12057393120585000, 1.19589965240923000, 1.29003461136252000, log(-1), 1.36810085170935000, 1.36361197989214000, 1.73639650227664000, log(-1.0)};
    float actual[8];
    vector8f av;
    vector8f cv;
    av.load_a(a);
    cv = log(av);
    cv.store_a(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_NEAR(log(a[idx]), actual[idx],0.0001);

}

TEST(SIMD_Test, vector8f_sqrt) {
    float a[8]         = {13.2, 15.7, 19.5, 1.5, 23.34, 23.1, 54.5, 88};
    // float expected[8]  = {1.12057393120585000, 1.19589965240923000, 1.29003461136252000, log(-1), 1.36810085170935000, 1.36361197989214000, 1.73639650227664000, log(-1.0)};
    float actual[8];
    vector8f av;
    vector8f cv;
    av.load_a(a);
    cv = sqrt(av);
    cv.store_a(actual);
    for(size_t idx=0; idx<8; idx++)
        EXPECT_NEAR(sqrt(a[idx]), actual[idx],0.0001);

}

#endif