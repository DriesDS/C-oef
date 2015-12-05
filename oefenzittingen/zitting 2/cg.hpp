#ifndef glas2_iterative_krylov_cg_hpp
#define glas2_iterative_krylov_cg_hpp

#include <cassert>
#include <type_traits>

#include "vector_sum.hpp"
#include "vector.hpp"

namespace tws {

  //
  // This is code for the conjugate gradient Krylov method for solving a symmetric linear system (that is guaranteed to work for positive matrices)
  //
  // Stopcriterion:
  //
  //   number of iterations <= maximum number of iterations max_it
  //   ||r|| <= tolerance
  //
  // Input
  //   op:
  //     binary functor:
  //         op( x, y ) computes y = A * x, i.e., op is a linear operator
  //     where x and y are Vectors.
  //
  //   r takes the right-hand side.
  //   is_vector<R> is equivalent to std::true_type
  //
  //   x takes the initial solution.
  //   is_vector<X> is equivalent to std::true_type
  //
  //   tolerance: is the residual tolerance of the linear solver, see further.
  //   maxit: is the maximum number of iterations.
  //
  // Output:
  //   r is the residual of the final solution: r = b - A * x
  //   x is the solution.
  //

  template <typename X, typename R, typename Op>
  void cg( Op const& op, X& x, R& r, double const& tolerance, int max_it ) {
    typedef double value_type ;
    typedef vector container_type ;

    assert( x.size() == r.size() ) ;

    container_type  p( x.size() ) ;
    container_type  q( x.size() ) ;

    // Compute residual
    op( x, q ) ;
    r -= q ;

    value_type rho2 ;
    value_type rho1 ;

    for ( int it=0 ; it<max_it; ++it ) {
      rho1 = inner_prod( r, r ) ;
      if ( rho1 < tolerance*tolerance ) return ;

      if (it==0) {
        p = r ;
      } else {
        p = r + (rho1/rho2) * p ;
      }
      op( p, q ) ;
      value_type alpha = rho1 / inner_prod( p, q ) ;
      x += alpha * p ;
      r -= alpha * q ;

      rho2 = rho1 ;
    }
  } // cg
}

#endif
