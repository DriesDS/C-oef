
#include <typeinfo>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "vector_sum.hpp"
#include "vector.hpp"
#include "cg.hpp"

struct matvec {
	double m;
	matvec(const double m = 2) : m(m) { }
	
	template <typename X, typename Y>
	void operator() ( X const& x, Y& y ) const {
    int n = x.size();
    if (n > 1) {
      y[0] = (-2*x[0]+x[1])*pow(n+1,2);
      y[n-1] = (-2*x[n-1] + x[n-2])*pow(n+1,2);
      for (std::size_t i=1; i<x.size()-1; ++i) {
        y[i] = (x[i-1] -2*x[i] + x[i+1])*pow(n+1,2);
      }
    } else {
      // scalar version of matrix A
      y[0] = -2*x[0]*pow(n+1,2);
    }
  }
} ;

int main() {
  int n = 128 ;
  tws::vector b(n) ;
  tws::vector x(n) ;
  tws::vector res(n) ;
  tws::vector err(n) ;
  double prod;
  double maxerr;
  double id;
  bool same;

  for (int i=0; i<x.size(); ++i) {
    x[i] = i ;
    b[i] = 2 ;
    err[i] = 0 ;
  }

  prod = inner_prod(x,b);
  std::cout << prod << std::endl;

  std::cout << x + 2*b - x -2*b + b*x - 2*x << std::endl;
  std::cout << prod << std::endl ;

  return 0 ;
}
