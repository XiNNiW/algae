#include "../../src/core/simd/simd_config.h"
#if SSE_INSTR_SET>6
#include <gtest/gtest.h>

#include "../../src/core/simd/simd.h"
#include <cmath>

TEST(SIMD_Test, vector4d_add) {
    double a[4]         = {13.2 , 15.7, 19.5, 1.5,  };
    double b[4]         = {1.11 , 21.5, 71.9, 9.13, };
    double expected[4]  = {14.31, 37.2, 91.40, 10.63};
    double actual[4];
    vector4d av, bv, cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av + bv;
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx], actual[idx], 0.0001);

}
TEST(SIMD_Test, vector4d_add_left_scalar) {
    double a[4]         = {13.2 , 15.7, 19.5, 1.5};
    double expected[4]  = {14.31,	16.81,	20.61,	2.61};
    double actual[4];
    vector4d av, bv, cv;
    av.load_a(a);
    cv = av + vector4d(1.11);
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx], actual[idx], 0.0001);
}
TEST(SIMD_Test, vector4d_add_right_scalar) {
    double a[4]         = {13.2 , 15.7, 19.5, 1.5};
    double expected[4]  = {14.31,	16.81,	20.61,	2.61};
    double actual[4];
    vector4d av, cv;
    av.load_a(a);
    cv = vector4d(1.11) + av;
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx], actual[idx], 0.0001);

}

TEST(SIMD_Test, vector4d_sub) {
    double a[4]         = {13.2 , 15.7, 19.5, 1.5};
    double b[4]         = {1.11 , 21.5, 71.9, 9.13};
    double expected[4]  = {12.09,-5.8,-52.4,-7.63};
    double actual[4];
    vector4d av, bv, cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av - bv;
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector4d_sub_left_scalar) {
    double a[4]         = {13.2 , 15.7, 19.5, 1.5};
    double expected[4]  = {12.09,	14.59,	18.39,	0.39};
    double actual[4];
    vector4d av, cv;
    av.load_a(a);
    cv = av - vector4d(1.11);
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector4d_sub_right_scalar) {
    double a[4]         = {13.2 , 15.7, 19.5, 1.5};
    double expected[4]  = {-12.09,	-14.59,	-18.39,	-0.39};
    double actual[4];
    vector4d av, cv;
    av.load_a(a);
    cv = vector4d(1.11) - av;
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);

}

TEST(SIMD_Test, vector4d_mul) {
    double a[4]         = {13.2 , 15.7, 19.5, 1.5};
    double b[4]         = {1.11 , 21.5, 71.9, 9.13};
    double expected[4]  = {14.652, 337.55,1402.05,13.695};
    double actual[4];
    vector4d av, bv, cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av * bv;
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector4d_mul_left_scalar) {
    double a[4]         = {13.2 , 15.7, 19.5, 1.5};
    double expected[4]  = {14.652,17.427,21.645,1.665};
    double actual[4];
    vector4d av, cv;
    av.load_a(a);
    cv = av * vector4d(1.11);
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector4d_mul_right_scalar) {
    double a[4]         = {13.2 , 15.7, 19.5, 1.5};
    double expected[4]  = {14.652,17.427,21.645,1.665};
    double actual[4];
    vector4d av, cv;
    av.load_a(a);
    cv = vector4d(1.11) * av;
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);

}

TEST(SIMD_Test, vector4d_div) {
    double a[4]         = {13.2 , 15.7, 19.5, 1.5, };
    double b[4]         = {1.11 , 21.5, 71.9, 9.13,};
    double expected[4]  = {11.89189189189190000,0.73023255813953500,0.27121001390820600,0.16429353778751400,};
    double actual[4];
    vector4d av, bv, cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av / bv;
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector4d_div_left_scalar) {
    double a[4]         = {13.2 , 15.7, 19.5, 1.5};
    double expected[4]  = {11.89189189189190000,14.14414414414410000,17.56756756756760000,1.35135135135135000};
    double actual[4];
    vector4d av, cv;
    av.load_a(a);
    cv = av / vector4d(1.11);
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);
}
TEST(SIMD_Test, vector4d_div_right_scalar) {
    double a[4]         = {13.2 , 15.7, 19.5, 1.5};
    double expected[4]  = {0.08409090909090910,0.07070063694267520,0.05692307692307690,0.74000000000000000,};
    double actual[4];
    vector4d av, cv;
    av.load_a(a);
    cv = vector4d(1.11) / av;
    cv.store_a(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_NEAR(expected[idx],actual[idx],0.0001);

}

TEST(SIMD_Test, vector4d_div_0) {
    double a[4]         = {13.2 , 15.7, 19.5, 1.5};
    double b[4]         = {1.11 , 0, 71.9, 9.13};
    double expected[4]  = {11.89189189189190000,0,0.27121001390820600,0.16429353778751400};
    double actual[4];
    vector4d av, bv, cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av / bv;
    cv.store_a(actual);
    
    EXPECT_NEAR(expected[0],actual[0],0.0001);
    EXPECT_TRUE(std::isinf(actual[1]));
    EXPECT_NEAR(expected[2],actual[2],0.0001);
    EXPECT_NEAR(expected[3],actual[3],0.0001);

}

// //comparison

TEST(SIMD_Test, vector4d_gt) {
    double a[4]         = {13.2 , 15.7, 19.5, 1.5};
    double b[4]         = {1.11 , 21.5, 71.9, 9.13};
    bool expected[4]   = {true, false, false, false};
    bool actual[4];
    vector4d av, bv;
    vector4db cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av > bv;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector4d_gt_right_scalar) {
    double a[4]         = {0.5 , 15.7, 19.5, 1.5};
    bool expected[4]  = {false, true, true, true};
    bool actual[4];
    vector4d av, bv;
    vector4db cv;
    av.load_a(a);
    cv = av > vector4d(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector4d_gt_left_scalar) {
    double a[4]         = {0.5 , 15.7, 19.5,  1.11};
    bool expected[4]  = {true, false, false, false};
    bool actual[4];
    vector4d av;
    vector4db cv;
    av.load_a(a);
    cv = vector4d(1.11) > av;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector4d_gte) {
    double a[4]         = {13.2 , 15.7, 71.9, 1.5};
    double b[4]         = {1.11 , 21.5, 71.9, 9.13};
    bool expected[4]  =  {true, false, true, false};
    bool actual[4];
    vector4d av, bv;
    vector4db cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av >= bv;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector4d_gte_right_scalar) {
    double a[4]         = {0.5 , 15.7, 71.9, 1.5};
    bool expected[4]   = {false, true, true, true};
    bool actual[4];
    vector4d av, bv;
    vector4db cv;
    av.load_a(a);
    cv = av >= vector4d(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector4d_gte_left_scalar) {
    double a[4]         = {1.11 , -3, 71.9, 1.5};
    bool expected[4]   = {true, true, false, false};
    bool actual[4];
    vector4d av;
    vector4db cv;
    av.load_a(a);
    cv = vector4d(1.11) >= av;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector4d_lt) {
    double a[4]         = {13.2 , 15.7, 19.5, 1.5};
    double b[4]         = {1.11 , 21.5, 71.9, 9.13 };
    bool expected[4]   = {false, true, true, true};
    bool actual[4];
    vector4d av, bv;
    vector4db cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av < bv;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector4d_lt_right_scalar) {
    double a[4]         = {0.5, 15.7, 19.5, 1.5};
    bool expected[4]   = {true, false, false, false};
    bool actual[4];
    vector4d av, bv;
    vector4db cv;
    av.load_a(a);
    cv = av < vector4d(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector4d_lt_left_scalar) {
    double a[4]         = {0.5,   15.7, 19.5, 1.5};
    bool expected[4]   = {false, true, true, true};
    bool actual[4];
    vector4d av;
    vector4db cv;
    av.load_a(a);
    cv = vector4d(1.11) < av;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector4d_lte) {
    double a[4]         = {13.2 , 15.7, 19.5, 1.5,};
    double b[4]         = {1.11 , 21.5, 19.5, 9.13};
    bool expected[4]   = {false, true, true, true};
    bool actual[4];
    vector4d av, bv;
    vector4db cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av <= bv;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector4d_lte_right_scalar) {
    double a[4]         = {0.5, 15.7, 19.5, 1.5};
    bool expected[4]   = {true, false, false, false};
    bool actual[4];
    vector4d av, bv;
    vector4db cv;
    av.load_a(a);
    cv = av <= vector4d(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector4d_lte_left_scalar) {
    double a[4]         = {0.5,   15.7, 19.5, 1.5};
    bool expected[4]   = {false, true, true, true};
    bool actual[4];
    vector4d av;
    vector4db cv;
    av.load_a(a);
    cv = vector4d(1.11) <= av;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++){
        EXPECT_EQ(expected[idx], actual[idx]); std::cout<<a[idx]<<"-"<<expected[idx]<<", "<<actual[idx]<<"\n";
    }

}


TEST(SIMD_Test, vector4d_eq) {
    double a[4]         = {13.2 , 15.7, 71.9, 1.5   };
    double b[4]         = {1.11 , 15.7, 71.9, 9.13  };
    bool expected[4]   = {false, true, true, false, };
    bool actual[4];
    vector4d av, bv;
    vector4db cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av == bv;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector4d_eq_right_scalar) {
    double a[4]        =  {0.5 , 1.11, 19.5, 1.11  };
    bool expected[4]  =  {false, true, false, true};
    bool actual[4];
    vector4d av, bv;
    vector4db cv;
    av.load_a(a);
    cv = av == vector4d(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector4d_eq_left_scalar) {
    double a[4]         = {0.5 , 15.7, 19.5, 1.11  ,};
    bool expected[4]  =  {false, false, false, true};
    bool actual[4];
    vector4d av;
    vector4db cv;
    av.load_a(a);
    cv = vector4d(1.11) == av;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

TEST(SIMD_Test, vector4d_neq) {
    double a[4]         = {13.2 , 15.7, 71.9, 1.5  };
    double b[4]         = {1.11 , 15.7, 71.9, 9.13 };
    bool expected[4]   = {true, false, false, true};
    bool actual[4];
    vector4d av, bv;
    vector4db cv;
    av.load_a(a);
    bv.load_a(b);
    cv = av != bv;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}
TEST(SIMD_Test, vector4d_neq_right_scalar) {
    double a[4]         = {0.5 , 1.11, 19.5, 1.11  };
    bool expected[4]  =  {true, false, true, false};
    bool actual[4];
    vector4d av, bv;
    vector4db cv;
    av.load_a(a);
    cv = av != vector4d(1.11);
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);
}
TEST(SIMD_Test, vector4d_neq_left_scalar) {
    double a[4]         = {0.5 , 15.7, 19.5, 1.11 };
    bool expected[4]  =  {true, true, true, false};
    bool actual[4];
    vector4d av;
    vector4db cv;
    av.load_a(a);
    cv = vector4d(1.11) != av;
    cv.store(actual);
    for(size_t idx=0; idx<4; idx++)
        EXPECT_EQ(expected[idx], actual[idx]);

}

#endif