
#include <typeinfo>
#include <iostream>
#include <algorithm>

#include "vector_sum.hpp"
#include "vector.hpp"
#include "cg.hpp"

struct matvec {
	double m;
	matvec(const double m = 2) : m(m) { }
	
	template <typename X, typename Y>
	void operator() ( X const& x, Y& y ) const {
		for (int i=0; i<x.size(); ++i) {
			y[i] = x[i]/m ;
		}
	}
} ;

int main() {
  tws::vector b(100) ;
  tws::vector x(100) ;
  double prod;

  for (int i=0; i<x.size(); ++i) {
   b[i] = 1;
   x[i] = 3;
  }

  x -= b;
  x += b;
  prod = inner_prod(b,x);
  // x -= x;
  // x = b;
  
  tws::cg( matvec(), x, b, 1.e-5, 40 ) ;

  std::cout << x << std::endl ;
  std::cout << prod << std::endl ;

  return 0 ;
}
