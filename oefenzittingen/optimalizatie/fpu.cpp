#include <iostream>
#include <cmath>
#include <limits>
#include <iomanip>

template <typename T>
T f()
{
    T x = 1;
    T y = pow(T(2), T(-25));
    T z = 1;
    return (x + y) - z;
}

int main()
{
    std::cout << std::setprecision(std::numeric_limits<long double>::digits10+1) 
              << std::scientific;
    
    float wf = f<float>();
    std::cout << "float result       = " << wf << std::endl;
    double wd = f<double>();
    std::cout << "double result      = " << wd << std::endl;
    long double we = f<long double>();
    std::cout << "long double result = " << we << std::endl;

    return 0;
}
