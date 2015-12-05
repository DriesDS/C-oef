
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
  tws::vector b(100) ;
  tws::vector x(100) ;
  tws::vector s(100) ;
  tws::vector err(100) ;
  double prod;
  double maxerr;

  //initialize x and b appropriately
  for (int i=0; i<x.size(); ++i) {
    id = static_cast<double>(i+1);
    x[i] = 0;
    b[i] = -(3*id/n+pow(id/n,2))*exp(id/n);
    s[i] = (ild/n)*exp(ild/n)-pow(ild/n,2)*exp(ild/n);
  }

  tws::cg( matvec(), x, b, 1.e-5, 40 ) ;

  maxerr = 0;
  for(int i=0; i<x.size(); ++i) {
    err[i] = abs( s[i]-x[i] );
    if ( abs( s[i]-x[i] ) > maxerr ) {
      maxerr = abs( s[i]-x[i] ) ;
    }
  }

  std::cout << "maximal error: " << maxerr << std::endl;
  std::cout << x << std::endl ;
  std::cout << prod << std::endl ;

  return 0 ;
}
