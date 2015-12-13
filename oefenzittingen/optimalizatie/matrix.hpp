#ifndef DIRKN_MATRIX_HPP
#define DIRKN_MATRIX_HPP

#include <vector>
#include <numeric>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <typeinfo>
#include <type_traits>
#include <algorithm>

namespace tws {

    template <typename T>
    class Diagonal
    {
        protected:
            const std::size_t rows_;
            const std::size_t cols_;
            std::vector<T> data_;
        public:
            typedef T value_type;
            Diagonal(std::size_t rows, std::size_t cols, const T& init)
            : rows_(rows)
            , cols_(cols)
            , data_(std::min(rows, cols), init)
            {
            }

            const T/*&*/ operator()(std::size_t i, std::size_t j) const {
                return (i == j) ? data_[i] : 0; // can't return reference to const 0
            }
            const T& operator()(std::size_t i) const {
                return data_[i];
            }
            T& operator()(std::size_t i) {
                return data_[i];
            }
    };

    template <typename T>
    class ColumnMajor
    {
        protected:
            const std::size_t rows_;
            const std::size_t cols_;
            std::vector<T> data_;
        public:
            typedef T value_type;
            ColumnMajor(std::size_t rows, std::size_t cols, const T& init)
            : rows_(rows)
            , cols_(cols)
            , data_(rows*cols, init)
            {
            }

            T& operator()(std::size_t i, std::size_t j) {
                std::size_t k = j*rows_ + i;
                assert(k < data_.size());
                return data_[k];
            }
            const T& operator()(std::size_t i, std::size_t j) const {
                std::size_t k = j*rows_ + i;
                assert(k < data_.size());
                return data_[k];
            }
    };

    template <class Shape>
    class Matrix : public Shape
    {
        public:
            typedef typename Shape::value_type value_type;

            Matrix(std::size_t rows, std::size_t cols, const value_type& fillwith = 0) 
            : Shape(rows, cols, fillwith)
            {
            }

            std::size_t rows() const { return this->rows_; }
            std::size_t cols() const { return this->cols_; }

       Matrix& operator=(const Matrix& A)
            {
                assert((rows() == A.rows()) && (cols() == A.cols()));
                std::copy(begin(A.data_), end(A.data_), begin(this->data_));
                return *this;
		}
      
        template <typename MatrixConcept>
            Matrix& operator=(const MatrixConcept& A)
            {
                assert((rows() == A.rows()) && (cols() == A.cols()));
                for(std::size_t j=0; j < A.cols(); ++j)
                for(std::size_t i=0; i < A.rows(); ++i)
                    this->operator()(i,j) = A(i,j);
                return *this;
		}

            std::ostream& dump(std::ostream& out) const
            {
                out << "_Z" << typeid(*this).name() << " [" << rows() << "x" << cols() << "] @" << this;
                return out;
            }

            static int print_width;
            friend std::ostream& operator<<(std::ostream& out, const Matrix& A)
            {
                A.dump(out);
                out << std::endl;
                for(std::size_t i=0; i < A.rows(); ++i) {
                    for(std::size_t j=0; j < A.cols(); ++j)
                        out << std::setw(print_width) << A(i,j) << " ";
                    out << std::endl;
                }
                return out;
            }
    };

    template <class Shape>
    int Matrix<Shape>::print_width = 5;


    template <class Matrix>
    typename Matrix::value_type max_norm(const Matrix& A)
    {
        using std::abs; using std::max;
        auto s = A(0, 0);
        for(std::size_t j=0; j < A.cols(); ++j)
        for(std::size_t i=0; i < A.rows(); ++i)
            s = max(s, A(i,j));
        return s;
    }

    /*
    template <typename T>
    T max_norm(const Matrix<ColMajor<T>>& A)
    {
        ...
    }
    */

    template <class M1, class M2>
    typename std::common_type<typename M1::value_type, typename M2::value_type>::type
    inner_product(const M1& A, const M2& B)
    {
#ifdef UNROLL
        auto s1 = A(0, 0) * B(0, 0);
        auto s2 = A(1, 0) * B(1, 0);
        auto s3 = A(2, 0) * B(2, 0);
        auto s4 = A(3, 0) * B(3, 0);
        for(std::size_t j=0; j < A.cols(); ++j)
        for(std::size_t i=0; i < A.rows(); i+=4)
        {
            s1 += A(i,j) * B(i,j);
            s2 += A(i+1,j) * B(i+1,j);
            s3 += A(i+2,j) * B(i+2,j);
            s4 += A(i+3,j) * B(i+3,j);
        }
        return s1 + s2 + s3 + s4;
#else
        auto s = A(0, 0) * B(0, 0);
        for(std::size_t j=0; j < A.cols(); ++j)
        for(std::size_t i=0; i < A.rows(); ++i)
            s += A(i,j) * B(i,j);
        return s;
#endif
    }

}

#endif
