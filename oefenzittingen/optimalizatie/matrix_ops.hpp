#ifndef DIRKN_MATRIX_OPS
#define DIRKN_MATRIX_OPS

#include <cassert>
#include "matrix.hpp"

// to not make this too complicated we can only add matrices of the same shape

template <typename Shape>
tws::Matrix<Shape> operator+(tws::Matrix<Shape> A, const tws::Matrix<Shape>& B)
{
    assert(A.rows() == B.rows() && A.cols() == B.cols());
    for(std::size_t i = 0; i < A.rows(); ++i)
        for(std::size_t j = 0; j < A.cols(); ++j)
            A(i, j) += B(i, j);
    return A;
}

template <typename Shape>
tws::Matrix<Shape> operator*(tws::Matrix<Shape> A, const tws::Matrix<Shape>& B)
{
    assert(A.rows() == B.rows() && A.cols() == B.cols());
    for(std::size_t i = 0; i < A.rows(); ++i)
        for(std::size_t j = 0; j < A.cols(); ++j)
            A(i, j) *= B(i, j);
    return A;
}

#endif
