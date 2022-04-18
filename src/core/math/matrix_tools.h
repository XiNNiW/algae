#pragma once
#include <iostream>

namespace algae::dsp::core::math {

template<typename sample_t, size_t ROWS, size_t COLS>
const inline std::array<sample_t, COLS*ROWS> transpose(const std::array<sample_t, ROWS*COLS> &A){
    std::array<sample_t, COLS*ROWS> A_trans;
    
    
    for (size_t n = 0; n<ROWS; n++) {
        for (size_t m = 0; m<COLS; m++) {
             A_trans[m*ROWS + n] = A[n*COLS + m];
        }

    }
 
    return A_trans;
}


template<typename sample_t, size_t ROWS1, size_t COLS1, size_t COLS2, size_t ROWS2 = COLS1>
const inline std::array<sample_t, ROWS1*COLS2> matrix_mult(
    const std::array<sample_t, ROWS1*COLS1> &lhs, 
    const std::array<sample_t, ROWS2*COLS2> &rhs
){
    std::array<sample_t, ROWS1*COLS2> result;
    
    for(size_t r1=0; r1<ROWS1; r1++){
        for(size_t c2=0; c2<COLS2; c2++){
            result[c2+r1*COLS2] = 0;
            for(size_t c1=0; c1<COLS1; c1++){
                result[c2+r1*COLS2]+=lhs[r1*COLS1+c1]*rhs[c1*COLS2+c2];
            }
        }
    }

    return result;

}





// Function to get cofactor 
template<typename sample_t>
void getCofactor(
    const sample_t* A, 
    sample_t* temp, 
    const size_t& p, 
    const size_t& q, 
    const size_t& N
)
{
  
    size_t i = 0, j = 0;
 
    // Looping for each element of the matrix
    for (size_t row = 0; row < N; row++)
    {
        for (size_t col = 0; col < N; col++)
        {
            // Copying into temporary matrix only those element
            // which are not in given row and column
            if (row != p && col != q)
            {
                temp[i * N + j++] = A[row * N + col];
 
                // Row is filled, so increase row index and
                // reset col index
                if (j == row - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

// Recursive function for finding determinant of matrix.
template<typename sample_t>
const inline sample_t determinant(const sample_t* A, const size_t& N)
{
    sample_t D = 0; // Initialize result
 
    // Base case : if matrix contains single element
    if (N == 1)
        return A[0];
 
    sample_t cofactors[N*N]; // To store cofactors
 
    int sign = 1; // To store sign multiplier
 
    // Iterate for each element of first row
    for (size_t f = 0; f < N; f++)
    {
        // Getting Cofactor of A[0][f]
        getCofactor<sample_t>(A, cofactors, 0, f, N);
        D += sign * A[f] * determinant<sample_t>(cofactors, N-1);
 
        // terms are to be added with alternate sign
        sign = -sign;
    }
 
    return D;
}



// Function to get adjoint
template<typename sample_t, size_t N>
const inline std::array<sample_t, N*N> adjoint(const std::array<sample_t, N*N> &A)
{
    std::array<sample_t, N*N> adj;
    if (N==1){
        adj[0] = 1;
        return adj;
    }
    

    // temp is used to store cofactors 
    int sign = 1;
    std::array<sample_t, N*N> temp;
 
    for (size_t i=0; i<N; i++)
    {
        for (size_t j=0; j<N; j++)
        {
            // Get cofactor
            getCofactor<sample_t>(A.data(), temp.data(), i, j, N);
 
            // sign of adj positive if sum of row
            // and column indexes is even.
            sign = ((i+j)%2==0)? 1: -1;
 
            // Interchanging rows and columns to get the
            // transpose of the cofactor matrix
            adj[j * N + i] = (sign)*(determinant<sample_t>(temp.data(), N-1));
        }
    }

    return adj;
}

// Function to calculate and store inverse, returns false if
// matrix is singular
template<typename sample_t, size_t N>
const inline std::array<sample_t, N*N> inverse(const std::array<sample_t, N*N> &A)
{   
    std::array<sample_t, N*N> inv;
    // Find determinant of A[][]
    sample_t det = determinant<sample_t>(A.data(), N); 
    // if (det == 0)
    // {
    //     // cout << "Singular matrix, can't find its inverse";
    //     // return false;
    //     return inv;
    // }
 
    // Find adjoint
    std::array<sample_t, N*N> adj = adjoint<sample_t, N>(A);
 
    // Find Inverse using formula "inverse(A) = adj(A)/det(A)"
    for (size_t i=0; i<N; i++)
        for (size_t j=0; j<N; j++)
            inv[i * N + j] = adj[i * N + j]/sample_t(det);
 
    return inv;
}


template<typename sample_t, size_t N, size_t M>
const inline std::array<sample_t, M*N> moore_penrose_pseudoinverse(const std::array<sample_t, N*M> &A){
    

    // for(size_t n=0; n<N; n++){
    //     std::cout << "\n";
    //     for(size_t m=0; m<M; m++)
    //         std::cout << A[n*M + m] << ","; 
    // }
    // std::cout << "\n";

    // for(size_t n=0; n<N; n++){
    //     std::cout << "\n";
    //     for(size_t m=0; m<M; m++)
    //         std::cout << Atrans[n*M + m] << ","; 
    // }
    // std::cout << "\n";

    // for(size_t n=0; n<M; n++){
    //     std::cout << "\n";
    //     for(size_t m=0; m<M; m++)
    //         std::cout << matrix_mult<sample_t,M,N,M>(Atrans, A)[n*M + m] << ","; 
    // }
    // std::cout << "\n";

    //  for(size_t n=0; n<M; n++){
    //     std::cout << "\n";
    //     for(size_t m=0; m<M; m++)
    //         std::cout << inverse<sample_t,M>(
    //         matrix_mult<sample_t,M,N,M>(Atrans, A)
    //     )[n*M + m] << ","; 
    // }
    // std::cout << "\n";
    std::array<sample_t, M*N> Atrans = transpose<sample_t, N, M>(A);

        
    if(M<N){
        std::array<sample_t, M*N> At = matrix_mult<sample_t,M,M,N>(
            inverse<sample_t,M>(
                matrix_mult<sample_t,M,N,M>(Atrans, A)
            ),
            Atrans
        );
        
        return At;
    } else {
        std::array<sample_t, M*N> At = matrix_mult<sample_t,M,N,N>(
            Atrans,
            inverse<sample_t,N>(
                matrix_mult<sample_t,N,M,N>(A, Atrans)
            )
        );
        
        return At;
    }
    
}
 
}