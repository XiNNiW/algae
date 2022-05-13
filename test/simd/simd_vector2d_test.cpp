#include <gtest/gtest.h>
#include "../../src/core/simd/simd.h"
#include <cmath>

TEST(SIMD_Test, vector2d_add) {
    double a[2]         = {13.2 , 15.7};
    double b[2]         = {1.11 , 21.5};
    double expected[2]  = {14.31, 37.2};
    double actual[2];
    vector2d av, bv, cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av + bv;
    cv.store_a(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_NEAR(expected[idx], actual[idx], 0.0001);

}
TEST(SIMD_Test, vector2d_add_left_scalar) {
    double a[2]         = {13.2 , 15.7};
    double expected[2]  = {14.31 , 16.81};
    double actual[2];
    vector2d av, bv, cv;
    av.load_a(a);
    cv = av + vector2d(1.11);
    cv.store_a(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_NEAR(expected[idx], actual[idx], 0.0001);
}
TEST(SIMD_Test, vector2d_add_right_scalar) {
    double a[2]         = {13.2 , 15.7};
    double expected[2]  = {14.31 , 16.81};
    double actual[2];
    vector2d av, cv;
    av.load_a(a);
    cv = vector2d(1.11) + av;
    cv.store_a(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_NEAR(expected[idx], actual[idx], 0.0001);

}

TEST(SIMD_Test, vector2d_sub) {
    double a[2]         = {13.2 , 15.7};
    double b[2]         = {1.11 , 21.5};
    double expected[2]  = {12.09, -5.8};
    double actual[2];
    vector2d av, bv, cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av - bv;
    cv.store_a(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector2d_sub_left_scalar) {
    double a[2]         = {13.2 , 15.7};
    double expected[2]  = {12.09 , 14.59};
    double actual[2];
    vector2d av, cv;
    av.load_a(a);
    cv = av - vector2d(1.11);
    cv.store_a(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector2d_sub_right_scalar) {
    double a[2]         = {13.2 , 15.7 };
    double expected[2]  = {-12.09, -14.59};
    double actual[2];
    vector2d av, cv;
    av.load_a(a);
    cv = vector2d(1.11) - av;
    cv.store_a(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);

}

TEST(SIMD_Test, vector2d_mul) {
    double a[2]         = {13.2 , 15.7};
    double b[2]         = {1.11 , 21.5};
    double expected[2]  = {14.652,	337.55};
    double actual[2];
    vector2d av, bv, cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av * bv;
    cv.store_a(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector2d_mul_left_scalar) {
    double a[2]         = {13.2 , 15.7};
    double expected[2]  = {14.652, 17.427};
    double actual[2];
    vector2d av, cv;
    av.load_a(a);
    cv = av * vector2d(1.11);
    cv.store_a(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector2d_mul_right_scalar) {
    double a[2]         = {13.2 , 15.7};
    double expected[2]  = {14.652, 17.427};
    double actual[2];
    vector2d av, cv;
    av.load_a(a);
    cv = vector2d(1.11) * av;
    cv.store_a(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);

}

TEST(SIMD_Test, vector2d_div) {
    double a[2]         = {13.2 , 15.7};
    double b[2]         = {1.11 , 21.5};
    double expected[2]  = {11.8918918918919,	0.730232558139535};
    double actual[2];
    vector2d av, bv, cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av / bv;
    cv.store_a(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector2d_div_left_scalar) {
    double a[2]         = {13.2 , 15.7};
    double expected[2]  = {11.8918918918919,	14.1441441441441};
    double actual[2];
    vector2d av, cv;
    av.load_a(a);
    cv = av / vector2d(1.11);
    cv.store_a(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector2d_div_right_scalar) {
    double a[2]         = {13.2 , 15.7};
    double expected[2]  = {0.0840909090909091,	0.0707006369426752};
    double actual[2];
    vector2d av, cv;
    av.load_a(a);
    cv = vector2d(1.11) / av;
    cv.store_a(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);

}

TEST(SIMD_Test, vector2d_div_0) {
    double a[2]         = {13.2 , 15.7};
    double b[2]         = {1.11 , 0};
    double expected[2]  = {11.8918918918919,	0};
    double actual[2];
    vector2d av, bv, cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av / bv;
    cv.store_a(actual);
    
    EXPECT_NEAR(expected[0],actual[0],0.0001);
    EXPECT_TRUE(std::isinf(actual[1]));

}

//comparison

TEST(SIMD_Test, vector2d_gt) {
    double a[2]         = {13.2 , 15.7 };
    double b[2]         = {1.11 , 21.5 };
    bool expected[2]  = {true, false };
    bool actual[2];
    vector2d av, bv;
    vector2db cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av > bv;
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector2d_gt_right_scalar) {
    double a[2]         = {0.5 , 15.7};
    bool expected[2]  = {false, true};
    bool actual[2];
    vector2d av, bv;
    vector2db cv;
    av.load_a(a);
    cv = av > vector2d(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector2d_gt_left_scalar) {
    double a[2]         = {0.5 , 15.7 };
    bool expected[2]  = {true, false };
    bool actual[2];
    vector2d av;
    vector2db cv;
    av.load_a(a);
    cv = vector2d(1.11) > av;
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector2d_gte) {
    double a[2]         = {13.2 , 15.7};
    double b[2]         = {1.11 , 21.5};
    bool expected[2]   = {true, false };
    bool actual[2];
    vector2d av, bv;
    vector2db cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av >= bv;
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector2d_gte_right_scalar) {
    double a[2]         = {0.5 , 15.7 };
    bool expected[2]  = {false, true };
    bool actual[2];
    vector2d av, bv;
    vector2db cv;
    av.load_a(a);
    cv = av >= vector2d(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector2d_gte_left_scalar) {
    double a[2]         = {0.5 , 15.7};
    bool expected[2]  = {true, false};
    bool actual[2];
    vector2d av;
    vector2db cv;
    av.load_a(a);
    cv = vector2d(1.11) >= av;
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector2d_lt) {
    double a[2]         = {13.2 , 15.7 };
    double b[2]         = {1.11 , 21.5 };
    bool expected[2]  = {false, true };
    bool actual[2];
    vector2d av, bv;
    vector2db cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av < bv;
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector2d_lt_right_scalar) {
    double a[2]         = {0.5 , 15.7};
    bool expected[2]  = {true, false};
    bool actual[2];
    vector2d av, bv;
    vector2db cv;
    av.load_a(a);
    cv = av < vector2d(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector2d_lt_left_scalar) {
    double a[2]         = {0.5 , 15.7};
    bool expected[2]  = {false, true};
    bool actual[2];
    vector2d av;
    vector2db cv;
    av.load_a(a);
    cv = vector2d(1.11) < av;
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector2d_lte) {
    double a[2]         = {13.2 , 15.7};
    double b[2]         = {1.11 , 21.5};
    bool expected[2]  = {false, true};
    bool actual[2];
    vector2d av, bv;
    vector2db cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av <= bv;
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector2d_lte_right_scalar) {
    double a[2]         = {0.5 , 15.7};
    bool expected[2]  = {true, false};
    bool actual[2];
    vector2d av, bv;
    vector2db cv;
    av.load_a(a);
    cv = av <= vector2d(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector2d_lte_left_scalar) {
    double a[2]         = {0.5 , 15.7};
    bool expected[2]  = {false, true};
    bool actual[2];
    vector2d av;
    vector2db cv;
    av.load_a(a);
    cv = vector2d(1.11) <= av;
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}


TEST(SIMD_Test, vector2d_eq) {
    double a[2]         = {13.2 , 15.7};
    double b[2]         = {1.11 , 15.7};
    bool expected[2]  = {false, true};
    bool actual[2];
    vector2d av, bv;
    vector2db cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av == bv;
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector2d_eq_right_scalar) {
    double a[2]         = {0.5 , 1.11};
    bool expected[2]  =  {false, true};
    bool actual[2];
    vector2d av, bv;
    vector2db cv;
    av.load_a(a);
    cv = av == vector2d(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector2d_eq_left_scalar) {
    double a[2]         = {0.5 , 1.11};
    bool expected[2]  =  {false, true};
    bool actual[2];
    vector2d av;
    vector2db cv;
    av.load_a(a);
    cv = vector2d(1.11) == av;
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector2d_neq) {
    double a[2]         = {13.2 , 15.7};
    double b[2]         = {1.11 , 15.7};
    bool expected[2]  = {true, false};
    bool actual[2];
    vector2d av, bv;
    vector2db cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av != bv;
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector2d_neq_right_scalar) {
    double a[2]         = {0.5 , 1.11 };
    bool expected[2]  =  {true, false};
    bool actual[2];
    vector2d av, bv;
    vector2db cv;
    av.load_a(a);
    cv = av != vector2d(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector2d_neq_left_scalar) {
    double a[2]         = {0.5 , 1.11};
    bool expected[2]  =  {true, false};
    bool actual[2];
    vector2d av;
    vector2db cv;
    av.load_a(a);
    cv = vector2d(1.11) != av;
    cv.store(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector2d_sin) {
    double a[2]         = {13.2 , 15.7};
    double expected[2]  = {0.59207351470722300,0.00796318378593734};
    double actual[2];
    vector2d av;
    vector2d cv;
    av.load_a(a);
    cv = sin(av);
    cv.store_a(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_NEAR(expected[idx], actual[idx], 0.0001);

}

TEST(SIMD_Test, vector2d_cos) {
    double a[2]         = {13.2, 15.7};
    double expected[2]  = {0.80588395764045100, -0.99996829334934000};
    double actual[2];
    vector2d av;
    vector2d cv;
    av.load_a(a);
    cv = cos(av);
    cv.store_a(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_NEAR(expected[idx], actual[idx], 0.0001);

}

TEST(SIMD_Test, vector2d_exp_matches_single_number_version) {
    // significant precision loss for numbers greater than 60 or so....
    double a[2]         = {13.2, 15.7};
    // double expected[2]; // = {540364.93724669200000000, 6582992.58458373000000000, 294267566.04150900000000000, 0.22313016014843000, 13690938122.32320000000000000, 10769673371.15770000000000000, 466712318321364000000000.00000000000000000, 0.00000000000000000};
    double actual[2];
    vector2d av;
    vector2d cv;
    av.load_a(a);
    cv = exp(av);
    cv.store_a(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_NEAR(exp(a[idx]), actual[idx],0.0001);

}

TEST(SIMD_Test, vector2d_log) {
    double a[2]         = {13.2, 15.7};
    // double expected[2]  = {1.12057393120585000, 1.19589965240923000, 1.29003461136252000, log(-1), 1.36810085170935000, 1.36361197989214000, 1.73639650227664000, log(-1.0)};
    double actual[2];
    vector2d av;
    vector2d cv;
    av.load_a(a);
    cv = log(av);
    cv.store_a(actual);
    for(size_t idx=0; idx<2; idx++)
        EXPECT_NEAR(log(a[idx]), actual[idx],0.0001);

}