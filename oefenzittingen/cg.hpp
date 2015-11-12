#ifndef tws_cg_hpp
#define tws_cg_hpp

#include <vector>
#include <algorithm>
#include <cassert>
#include <glas2/iterative/krylov/cg.hpp>
#include <glas2/iterative/krylov/no_report.hpp>
#include <glas2/vector.hpp>
#include <glas2/bindings/vector.hpp>
#include <boost/numeric/bindings/std/vector.hpp>

namespace tws {

  template <typename MatVec, typename T>
  struct std_wrap {
    std_wrap( MatVec const& mat_vec, int n )
    : mat_vec_( mat_vec )
    , x_( n )
    , y_( n )
    {}

    template <typename X, typename Y>
    void operator() ( X const& x, Y& y ) const {
      std::copy( x.ptr(), x.ptr()+x.size(), x_.begin() ) ;
      mat_vec_( x_, y_ ) ;
      std::copy( y_.begin(), y_.end(), y.ptr() ) ;
    }

    MatVec const&            mat_vec_ ;
    mutable std::vector< T > x_ ; // This is not efficient, but it is only an exercise!
    mutable std::vector< T > y_ ;
  } ;

  template <typename MatVec, typename Vector>
  void cg( MatVec const& mat_vec, Vector& x, Vector& r ) {
    assert( x.size() == r.size() ) ;
    glas2::iterative::options opt ;
    opt.max_mat_vec_ = 30000 ;
    opt.relative_tolerance_ = 1e-20 ;
    glas2::iterative::no_report rep ;

    typedef typename Vector::value_type value_type ;
    std_wrap< MatVec, value_type > wrap_op( mat_vec, x.size() ) ;

    glas2::iterative::cg( wrap_op, glas2::bind_vector(x), glas2::bind_vector(r), rep, opt, glas2::current_backend() ) ;
  } // cg()

} // namespace tws

#endif
