#include <iostream>
#include "bench.hpp"

#include "matrix.hpp"
#ifdef EXPR
#  include "expr.hpp"
#else
#  include "matrix_ops.hpp"
#endif

const std::size_t rows = 400;
const std::size_t cols = 300;
 
typedef tws::Matrix<tws::ColumnMajor<double>> Matrix;
struct MatrixTest
{

    Matrix A;
    Matrix B;
    Matrix C;
    
    MatrixTest()
    : A(rows, cols, -1)
    , B(rows, cols, 2)
    , C(rows, cols, 0)
    {
    }

    void operator()()
    {
       C = A + (B + (A + A + A) + B) + B;       
    }

    void use_result()
    {
        std::cout << C(0,0) << ", " << C(rows-1, cols-1) << std::endl;
    }

};

 
int main()
{
    const size_t n = 50;
    const size_t warmup = 5;
    typedef TscTimer timer;
    Bench< MatrixTest, n, warmup, timer > b(std::cout); // writes test results immediatly to cout
    b.dump(std::cerr);
    return 0;
}

