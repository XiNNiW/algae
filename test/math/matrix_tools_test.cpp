#include <gtest/gtest.h>
#include "algae.h"
#include <iostream>

using algae::dsp::core::math::transpose;

TEST(MatrixTools_Test, transpose_square) { 
   
    std::array<double, 3*3> matrix = {
        1,2,3,
        4,5,6,
        7,8,9
    };
    std::array<double, 3*3> expected_transpose = {
        1,4,7,
        2,5,8,
        3,6,9
    };
    std::array<double, 3*3> actual_transpose = transpose<double,3,3>(matrix);
 
    for(size_t idx=0; idx<3*3; idx++){
        // std::cout << actual_transpose[idx] << ", ";
        EXPECT_FLOAT_EQ(expected_transpose[idx], actual_transpose[idx]);
    }

}

TEST(MatrixTools_Test, transpose_rect1) { 
   
    std::array<double, 2*3> matrix = {
        1,2,3,
        4,5,6,
    };
    std::array<double, 3*2> expected_transpose = {
        1,4,
        2,5,
        3,6
    };
    std::array<double, 3*2> actual_transpose = transpose<double,2,3>(matrix);
    
    for(size_t idx=0; idx<3*2; idx++){
        // std::cout << actual_transpose[idx] << ", ";
        EXPECT_FLOAT_EQ(expected_transpose[idx], actual_transpose[idx]);
    }

}

TEST(MatrixTools_Test, transpose_rect2) { 
   
    std::array<double, 3*2> matrix = {
        1,2,
        4,5,
        7,8
    };
    std::array<double, 2*3> expected_transpose = {
        1,4,7,
        2,5,8
    };
    std::array<double, 2*3> actual_transpose = transpose<double,3,2>(matrix);
    for(size_t idx=0; idx<2*3; idx++){
        // std::cout << actual_transpose[idx] << ", ";
        EXPECT_FLOAT_EQ(expected_transpose[idx], actual_transpose[idx]);
    }

}

using algae::dsp::core::math::matrix_mult;
TEST(MatrixTools_Test, matrix_mult_square) { 
   
    std::array<double, 3*3> lhs = {
        1,2,3,
        4,5,6,
        7,8,9
    };
    std::array<double, 3*3> rhs = {
        9,8,7,
        6,5,4,
        3,2,1
    };
    std::array<double, 3*3> expected = {
        (1*9+2*6+3*3),(1*8+2*5+3*2),(1*7+2*4+3*1),
        (4*9+5*6+6*3),(4*8+5*5+6*2),(4*7+5*4+6*1),
        (7*9+8*6+9*3),(7*8+8*5+9*2),(7*7+8*4+9*1),
    };
    std::array<double, 3*3> actual = matrix_mult<double,3,3,3>(lhs,rhs);
 
    for(size_t idx=0; idx<3*3; idx++){
        // std::cout << actual_transpose[idx] << ", ";
        EXPECT_FLOAT_EQ(expected[idx], actual[idx]);
    }

}

TEST(MatrixTools_Test, matrix_mult_rect1) { 
   
    std::array<double, 2*3> lhs = {
        1,2,3,
        4,5,6
    };
    std::array<double, 3*2> rhs = {
        9,8,
        6,5,
        3,2
    };
    std::array<double, 2*2> expected = {
        (1*9+2*6+3*3),(1*8+2*5+3*2),
        (4*9+5*6+6*3),(4*8+5*5+6*2),
    };
    std::array<double, 2*2> actual = matrix_mult<double,2,3,2>(lhs,rhs);
 
    for(size_t idx=0; idx<2*2; idx++){
        // std::cout << actual_transpose[idx] << ", ";
        EXPECT_FLOAT_EQ(expected[idx], actual[idx]);
    }

}

TEST(MatrixTools_Test, matrix_mult_rect2) { 
   
    std::array<double, 3*2> lhs = {
        1,2,
        4,5,
        7,8,
    };
    std::array<double, 2*3> rhs = {
        9,8,7,
        6,5,4
    };
    std::array<double, 3*3> expected = {
        (1*9+2*6),(1*8+2*5),(1*7+2*4),
        (4*9+5*6),(4*8+5*5),(4*7+5*4),
        (7*9+8*6),(7*8+8*5),(7*7+8*4),
    };
    std::array<double, 3*3> actual = matrix_mult<double,3,2,3>(lhs,rhs);
 
    for(size_t idx=0; idx<3*3; idx++){
        // std::cout << actual_transpose[idx] << ", ";
        EXPECT_FLOAT_EQ(expected[idx], actual[idx]);
    }

}

using algae::dsp::core::math::determinant;
TEST(MatrixTools_Test, determinant) { 
   
    std::array<double, 3*3> lhs = {
       3,0,1,
       1,2,5,
       -1,4,2
    };

    double expected = -42;
    double actual = determinant<double>(lhs.data(),3);
 
    ASSERT_FLOAT_EQ(expected, actual);

}

using algae::dsp::core::math::adjoint;
TEST(MatrixTools_Test, adjoint) { 
   
    std::array<double, 2*2> A = {
       2,3,
       1,4
    };

    std::array<double, 2*2> expected = {
       4,-3,
       -1,2
    };
     std::array<double, 2*2> actual = adjoint<double,2>(A);
 
    for(size_t idx=0; idx<2*2; idx++){
        // std::cout << actual_transpose[idx] << ", ";
        EXPECT_FLOAT_EQ(expected[idx], actual[idx]);
    }

}

using algae::dsp::core::math::inverse;
TEST(MatrixTools_Test, inverse) { 
   
    std::array<double, 3*3> A = {
       3,0,1,
       1,2,5,
       -1,4,2
    };

    std::array<double, 3*3> expected = {
       16.0/42.0, -4.0/42.0, 2.0/42.0,
       7.0/42.0,-7.0/42.0, 14.0/42.0,
       -6.0/42.0, 12.0/42.0, -6.0/42.0
    };

    std::array<double, 3*3> actual = inverse<double, 3>(A);
 
    for(size_t idx=0; idx<2*2; idx++){
        // std::cout << actual_transpose[idx] << ", ";
        EXPECT_FLOAT_EQ(expected[idx], actual[idx]);
    }

}

using algae::dsp::core::math::moore_penrose_pseudoinverse;
TEST(MatrixTools_Test, moore_penrose_inverse) { 
   
     std::array<double, 2*3> A = {
       3,0,1,
       1,2,5
    };

    std::array<double, 3*2> expected = {
       41.0/118.0, -7.0/118.0, 
       -8.0/118.0, 10.0/118.0,
       -5.0/118.0, 21.0/118.0
    };

    std::array<double, 3*2> actual = moore_penrose_pseudoinverse<double,2,3>(A);
 
    for(size_t idx=0; idx<2*3; idx++){
        // std::cout << actual_transpose[idx] << ", ";
        EXPECT_FLOAT_EQ(expected[idx], actual[idx]);
    }

}

using algae::dsp::core::math::moore_penrose_pseudoinverse;
TEST(MatrixTools_Test, moore_penrose_inverse_2) { 
   
     std::array<double, 3*2> A = {
       3,0,
       1,2,
       1,5
    };

    std::array<double, 2*3> expected = {
       29.0/90.0,5.0/90.0,-2.0/90.0,
       -7.0/90.0,5.0/90.0,16.0/90.0
    };

    std::array<double, 2*3> actual = moore_penrose_pseudoinverse<double,3,2>(A);
 
    for(size_t idx=0; idx<2*3; idx++){
        // std::cout << actual_transpose[idx] << ", ";
        EXPECT_FLOAT_EQ(expected[idx], actual[idx]);
    }

}


using algae::dsp::core::math::toeplitz_matrix_create;
TEST(MatrixTools_Test, toeplitz_matrix_create) {
    constexpr size_t N = 5;
    constexpr size_t SIZE = 2*N-1;

    std::array<double, SIZE> signal = {1,2,3,4,5,6,7,8,9};
    std::array<double, N*N> expected = {
        1,2,3,4,5,
        2,1,2,3,4,
        3,2,1,2,3,
        4,3,2,1,2,
        5,4,3,2,1,
    };
    std::array<double, N*N> actual = toeplitz_matrix_create<double, N>(signal);

    for(size_t idx=0; idx<N*N; idx++){
       
       EXPECT_FLOAT_EQ(expected[idx], actual[idx]);
    }
        
} 

TEST(MatrixTools_Test, toeplitz_matrix_create_2) {
    constexpr size_t N = 6;
    constexpr size_t SIZE = 2*N-1;

    std::array<double, SIZE> signal = {1,2,3,4,5,6,7,8,9,10,11};
    std::array<double, N*N> expected = {
        1,2,3,4,5,6,
        2,1,2,3,4,5,
        3,2,1,2,3,4,
        4,3,2,1,2,3,
        5,4,3,2,1,2,
        6,5,4,3,2,1,
    };
    std::array<double, N*N> actual = toeplitz_matrix_create<double, N>(signal);

    for(size_t idx=0; idx<N*N; idx++)
        EXPECT_FLOAT_EQ(expected[idx], actual[idx]);
} 

using algae::dsp::core::math::autocorrelation;
TEST(MatrixTools_Test, autocorrelation){
    std::array<double, 7> seq = {1, 2, 3, 4, 3, 4, 2};
    std::array<double, 7> expected = {59, 52, 42, 30, 17, 8, 2};
    std::array<double, 7> actual  = autocorrelation<double,7,6>(seq);

    for(size_t idx=0; idx<7; idx++)
        EXPECT_FLOAT_EQ(expected[idx],actual[idx]);
}