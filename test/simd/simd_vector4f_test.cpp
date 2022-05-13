#include <gtest/gtest.h>
#include "../../src/core/simd/simd.h"
#include <cmath>

TEST(SIMD_Test, vector4f_add) {
    float a[4]         = {13.2 , 15.7, 19.5, 1.5  };
    float b[4]         = {1.11 , 21.5, 71.9, 9.13 };
    float expected[4]  = {14.31, 37.2, 91.4, 10.63};
    float actual[4];
    vector4f av, bv, cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av + bv;
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx], actual[idx], 0.0001);

}
TEST(SIMD_Test, vector4f_add_left_scalar) {
    float a[4]         = {13.2 , 15.7, 19.5, 1.5  };
    float expected[4]  = {14.31 , 16.81, 20.61, 2.61  };
    float actual[4];
    vector4f av, bv, cv;
    av.load_a(a);
    cv = av + vector4f(1.11);
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx], actual[idx], 0.0001);
}
TEST(SIMD_Test, vector4f_add_right_scalar) {
    float a[4]         = {13.2 , 15.7, 19.5, 1.5  };
    float expected[4]  = {14.31 , 16.81, 20.61, 2.61  };
    float actual[4];
    vector4f av, cv;
    av.load_a(a);
    cv = vector4f(1.11) + av;
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx], actual[idx], 0.0001);

}

TEST(SIMD_Test, vector4f_sub) {
    float a[4]         = {13.2 , 15.7, 19.5, 1.5  };
    float b[4]         = {1.11 , 21.5, 71.9, 9.13 };
    float expected[4]  = {12.09, -5.8, -52.4, -7.63};
    float actual[4];
    vector4f av, bv, cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av - bv;
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector4f_sub_left_scalar) {
    float a[4]         = {13.2 , 15.7, 19.5, 1.5  };
    float expected[4]  = {12.09 , 14.59, 18.39, 0.39  };
    float actual[4];
    vector4f av, cv;
    av.load_a(a);
    cv = av - vector4f(1.11);
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector4f_sub_right_scalar) {
    float a[4]         = {13.2 , 15.7, 19.5, 1.5  };
    float expected[4]  = {-12.09, -14.59, -18.39, -0.39};
    float actual[4];
    vector4f av, cv;
    av.load_a(a);
    cv = vector4f(1.11) - av;
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);

}

TEST(SIMD_Test, vector4f_mul) {
    float a[4]         = {13.2 , 15.7, 19.5, 1.5  };
    float b[4]         = {1.11 , 21.5, 71.9, 9.13 };
    float expected[4]  = {14.652,	337.55, 1402.05, 13.695};
    float actual[4];
    vector4f av, bv, cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av * bv;
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector4f_mul_left_scalar) {
    float a[4]         = {13.2 , 15.7, 19.5, 1.5  };
    float expected[4]  = {14.652, 17.427, 21.645, 1.665};
    float actual[4];
    vector4f av, cv;
    av.load_a(a);
    cv = av * vector4f(1.11);
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector4f_mul_right_scalar) {
    float a[4]         = {13.2 , 15.7, 19.5, 1.5  };
    float expected[4]  = {14.652, 17.427, 21.645, 1.665};
    float actual[4];
    vector4f av, cv;
    av.load_a(a);
    cv = vector4f(1.11) * av;
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);

}

TEST(SIMD_Test, vector4f_div) {
    float a[4]         = {13.2 , 15.7, 19.5, 1.5  };
    float b[4]         = {1.11 , 21.5, 71.9, 9.13 };
    float expected[4]  = {11.8918918918919,	0.730232558139535,	0.271210013908206,	0.164293537787514};
    float actual[4];
    vector4f av, bv, cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av / bv;
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector4f_div_left_scalar) {
    float a[4]         = {13.2 , 15.7, 19.5, 1.5  };
    float expected[4]  = {11.8918918918919,	14.1441441441441,	17.5675675675676,	1.35135135135135};
    float actual[4];
    vector4f av, cv;
    av.load_a(a);
    cv = av / vector4f(1.11);
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector4f_div_right_scalar) {
    float a[4]         = {13.2 , 15.7, 19.5, 1.5  };
    float expected[4]  = {0.0840909090909091,	0.0707006369426752,	0.0569230769230769,	0.74};
    float actual[4];
    vector4f av, cv;
    av.load_a(a);
    cv = vector4f(1.11) / av;
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);

}

TEST(SIMD_Test, vector4f_div_0) {
    float a[4]         = {13.2 , 15.7, 19.5, 1.5  };
    float b[4]         = {1.11 , 0, 71.9, 9.13 };
    float expected[4]  = {11.8918918918919,	0,	0.271210013908206,	0.164293537787514};
    float actual[4];
    vector4f av, bv, cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av / bv;
    cv.store_a(actual);
    
    EXPECT_NEAR(expected[0],actual[0],0.0001);
    EXPECT_TRUE(std::isinf(actual[1]));
    EXPECT_NEAR(expected[2],actual[2],0.0001);
    EXPECT_NEAR(expected[3],actual[3],0.0001);

}

//comparison

TEST(SIMD_Test, vector4f_gt) {
    float a[4]         = {13.2 , 15.7, 19.5, 1.5  };
    float b[4]         = {1.11 , 21.5, 71.9, 9.13 };
    bool expected[4]  = {true, false, false, false};
    bool actual[4];
    vector4f av, bv;
    vector4fb cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av > bv;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector4f_gt_right_scalar) {
    float a[4]         = {0.5 , 15.7, 19.5, 1.5  };
    bool expected[4]  = {false, true, true, true};
    bool actual[4];
    vector4f av, bv;
    vector4fb cv;
    av.load_a(a);
    cv = av > vector4f(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector4f_gt_left_scalar) {
    float a[4]         = {0.5 , 15.7, 19.5, 1.5  };
    bool expected[4]  = {true, false, false, false};
    bool actual[4];
    vector4f av;
    vector4fb cv;
    av.load_a(a);
    cv = vector4f(1.11) > av;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector4f_gte) {
    float a[4]         = {13.2 , 15.7, 71.9, 1.5  };
    float b[4]         = {1.11 , 21.5, 71.9, 9.13 };
    bool expected[4]  = {true, false, true, false};
    bool actual[4];
    vector4f av, bv;
    vector4fb cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av >= bv;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector4f_gte_right_scalar) {
    float a[4]         = {0.5 , 15.7, 19.5, 1.11  };
    bool expected[4]  = {false, true, true, true};
    bool actual[4];
    vector4f av, bv;
    vector4fb cv;
    av.load_a(a);
    cv = av >= vector4f(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector4f_gte_left_scalar) {
    float a[4]         = {0.5 , 15.7, 19.5, 1.11  };
    bool expected[4]  = {true, false, false, true};
    bool actual[4];
    vector4f av;
    vector4fb cv;
    av.load_a(a);
    cv = vector4f(1.11) >= av;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector4f_lt) {
    float a[4]         = {13.2 , 15.7, 19.5, 1.5  };
    float b[4]         = {1.11 , 21.5, 71.9, 9.13 };
    bool expected[4]  = {false, true, true, true};
    bool actual[4];
    vector4f av, bv;
    vector4fb cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av < bv;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector4f_lt_right_scalar) {
    float a[4]         = {0.5 , 15.7, 19.5, 1.5  };
    bool expected[4]  = {true, false, false, false};
    bool actual[4];
    vector4f av, bv;
    vector4fb cv;
    av.load_a(a);
    cv = av < vector4f(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector4f_lt_left_scalar) {
    float a[4]         = {0.5 , 15.7, 19.5, 1.5  };
    bool expected[4]  = {false, true, true, true};
    bool actual[4];
    vector4f av;
    vector4fb cv;
    av.load_a(a);
    cv = vector4f(1.11) < av;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector4f_lte) {
    float a[4]         = {13.2 , 15.7, 71.9, 1.5  };
    float b[4]         = {1.11 , 21.5, 71.9, 9.13 };
    bool expected[4]  = {false, true, true, true};
    bool actual[4];
    vector4f av, bv;
    vector4fb cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av <= bv;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector4f_lte_right_scalar) {
    float a[4]         = {0.5 , 15.7, 19.5, 1.11  };
    bool expected[4]  = {true, false, false, true};
    bool actual[4];
    vector4f av, bv;
    vector4fb cv;
    av.load_a(a);
    cv = av <= vector4f(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector4f_lte_left_scalar) {
    float a[4]         = {0.5 , 15.7, 19.5, 1.11  };
    bool expected[4]  = {false, true, true, true};
    bool actual[4];
    vector4f av;
    vector4fb cv;
    av.load_a(a);
    cv = vector4f(1.11) <= av;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}


TEST(SIMD_Test, vector4f_eq) {
    float a[4]         = {13.2 , 15.7, 71.9, 1.5  };
    float b[4]         = {1.11 , 15.7, 71.9, 9.13 };
    bool expected[4]  = {false, true, true, false};
    bool actual[4];
    vector4f av, bv;
    vector4fb cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av == bv;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector4f_eq_right_scalar) {
    float a[4]         = {0.5 , 1.11, 19.5, 1.11  };
    bool expected[4]  =  {false, true, false, true};
    bool actual[4];
    vector4f av, bv;
    vector4fb cv;
    av.load_a(a);
    cv = av == vector4f(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector4f_eq_left_scalar) {
    float a[4]         = {0.5 , 15.7, 19.5, 1.11  };
    bool expected[4]  =  {false, false, false, true};
    bool actual[4];
    vector4f av;
    vector4fb cv;
    av.load_a(a);
    cv = vector4f(1.11) == av;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector4f_neq) {
    float a[4]         = {13.2 , 15.7, 71.9, 1.5  };
    float b[4]         = {1.11 , 15.7, 71.9, 9.13 };
    bool expected[4]  = {true, false, false, true};
    bool actual[4];
    vector4f av, bv;
    vector4fb cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av != bv;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector4f_neq_right_scalar) {
    float a[4]         = {0.5 , 1.11, 19.5, 1.11  };
    bool expected[4]  =  {true, false, true, false};
    bool actual[4];
    vector4f av, bv;
    vector4fb cv;
    av.load_a(a);
    cv = av != vector4f(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector4f_neq_left_scalar) {
    float a[4]         = {0.5 , 15.7, 19.5, 1.11  };
    bool expected[4]  =  {true, true, true, false};
    bool actual[4];
    vector4f av;
    vector4fb cv;
    av.load_a(a);
    cv = vector4f(1.11) != av;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

//TRIG

TEST(SIMD_Test, vector4f_sin) {
    float a[4]         = {13.2 , 15.7, 19.5, 1.5  };
    float expected[4]  = {0.59207351470722300,0.00796318378593734,0.60553986971960100,0.99749498660405500};
    float actual[4];
    vector4f av;
    vector4f cv;
    av.load_a(a);
    cv = sin(av);
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx], actual[idx], 0.0001);

}

TEST(SIMD_Test, vector4f_cos) {
    float a[4]         = {13.2, 15.7, 19.5, -1.5};
    float expected[4]  = {0.80588395764045100, -0.99996829334934000, 0.79581496981394400, 0.07073720166770290};
    float actual[4];
    vector4f av;
    vector4f cv;
    av.load_a(a);
    cv = cos(av);
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx], actual[idx], 0.0001);

}

TEST(SIMD_Test, vector4f_exp_matches_single_number_version) {
    // significant precision loss for numbers greater than 60 or so....
    float a[4]         = {13.2, 1.7, 1.9, -1.5};
    // float expected[4]; // = {540364.93724669200000000, 6582992.58458373000000000, 294267566.04150900000000000, 0.22313016014843000, 13690938122.32320000000000000, 10769673371.15770000000000000, 466712318321364000000000.00000000000000000, 0.00000000000000000};
    float actual[4];
    vector4f av;
    vector4f cv;
    av.load_a(a);
    cv = exp(av);
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(exp(a[idx]), actual[idx],0.0001);

}

TEST(SIMD_Test, vector4f_log) {
    float a[4]         = {13.2, 15.7, 19.5, 1.5};
    // float expected[4]  = {1.12057393120585000, 1.19589965240923000, 1.29003461136252000, log(-1), 1.36810085170935000, 1.36361197989214000, 1.73639650227664000, log(-1.0)};
    float actual[4];
    vector4f av;
    vector4f cv;
    av.load_a(a);
    cv = log(av);
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(log(a[idx]), actual[idx],0.0001);

}

