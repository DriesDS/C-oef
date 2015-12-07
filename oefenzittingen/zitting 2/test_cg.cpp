
#include <typeinfo>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <time.h>

#include "vector_sum.hpp"
#include "vector.hpp"

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
  int m = 100000 ;
  tws::vector<double> b(n) ;
  tws::vector<double> x(n) ;
  tws::vector<double> res(n) ;
  tws::vector<double> err(n) ;
  tws::vector<double> v(m) ;
  tws::vector<double> w(m) ;
  tws::vector<float> f(n) ;
  tws::vector<long double> ld(n) ;
  double prod;
  clock_t time0;
  clock_t time1;

  for (int i=0; i<x.size(); ++i) {
    x[i] = i ;
    b[i] = 2 ;
    err[i] = 0 ;
    f[i] = 0.5*i ;
    ld[i] = -0.1*i ;
  }

  prod = inner_prod(x,b);
  std::cout << x + 2*b - x -2*b + b*x - 2*x << std::endl;
  std::cout << prod << std::endl ;

  time0 = clock();
  for (int i=0; i<1000; ++i) { inner_prod(2.0*v, w); }
  time1 = clock();
  std::cout << "cpu_time of inner_prod(2.0*v, w) was: " << (time1-time0)/CLOCKS_PER_SEC << std::endl;

  time0 = clock();
  for (int i=0; i<1000; ++i) { 2.0*inner_prod(v, w); }
  time1 = clock();
  std::cout << "cpu_time of 2.0*inner_prod(v, w) was: " << (time1-time0)/CLOCKS_PER_SEC << std::endl;

  auto sum = f+ld;
  std::cout << typeid(sum[1]).name() << std::endl;

  return 0 ;
}
