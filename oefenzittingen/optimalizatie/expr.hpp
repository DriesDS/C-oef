#ifndef DIRKN_EXPR_HPP
#define DIRKN_EXPR_HPP

#include <functional>
#include <cassert>
#include <cstdlib>
#include <type_traits>
#include <typeinfo>
#include <iostream>

#include "matrix.hpp"

namespace tws {

    // some trickery
    template <typename T>
    struct is_BinX
    {
        const static bool value = false;
    };
    // forward decl
    template <typename Op, typename Left, typename Right> struct BinX;
    // specialization
    template <typename Op, typename Left, typename Right>
    struct is_BinX< BinX<Op, Left, Right> >
    {
        const static bool value = true;
    };


    template <typename Op, typename Left, typename Right>
    struct BinX
    {
        typedef typename std::common_type<typename Right::value_type, typename Left::value_type>::type value_type;

        typedef typename std::conditional< is_BinX<Left>::value , const Left , const Left&  >::type Left_t;
        typedef typename std::conditional< is_BinX<Right>::value, const Right, const Right& >::type Right_t;

        Left_t left;
        Right_t right;

        BinX(Left_t left, Right_t right)
        : left(left)
        , right(right)
        {
            assert((left.rows() == right.rows()) && (left.cols() == right.cols()));
        }

        value_type operator()(std::size_t i, std::size_t j) const
        {
            return Op()(left(i,j), right(i,j));
        }

        std::size_t rows() const
        {
            return right.rows();
        }

        std::size_t cols() const
        {
            return right.cols();
        }

        const char* type() const
        {
            return typeid(value_type).name();
        }

        std::ostream& dump(std::ostream& out) const
        {
            out << "BinX@" << this << "(type=" << type() << ", left=";
            left.dump(out);
            out << ", right=";
            right.dump(out);
            out << ")";
            return out;
        }

        friend std::ostream& operator<<(std::ostream& out, const BinX& X)
        {
            return X.dump(out);
        }

    };

    template <typename Left, typename Right>
    BinX< std::plus<typename std::common_type<typename Right::value_type, typename Left::value_type>::type>, Left, Right> 
    operator+(const Left& left, const Right& right)
    {
        return BinX<std::plus<typename std::common_type<typename Right::value_type, typename Left::value_type>::type>, Left, Right>(left, right);
    }

    template <typename Left, typename Right>
    BinX< std::multiplies<typename std::common_type<typename Right::value_type, typename Left::value_type>::type>, Left, Right> 
    operator*(const Left& left, const Right& right)
    {
        return BinX<std::multiplies<typename std::common_type<typename Right::value_type, typename Left::value_type>::type>, Left, Right>(left, right);
    }

}

#endif
