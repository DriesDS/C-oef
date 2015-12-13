#include <iostream>
#include <vector>
#include "bench.hpp"

const std::size_t N = 1 << 10;

struct SumTest
{
    double sum;
    std::vector<double> a;
    
    SumTest()
    : a(N, -1)
    {
    }

    void operator()()
    {
        sum = 0;
        for(std::size_t i = 0; i < N/4; i=++) {
            sum += a[i*4];
            sum += a[i*4+1];
            sum += a[i*4+2];
            sum += a[i*4+3];
        }
    }
    
    void use_result()
    {
        std::cout << sum << std::endl;
    }
    
};

int main()
{
    const size_t n = 50;      // number of experiments
    const size_t warmup = 5;  // number of warmup experiments for cache loads
    typedef TscTimer timer;   // use TSC timer
    Bench< SumTest, n, warmup, timer > b(std::cout);
    b.dump(std::cerr);        // output raw timing data to stderr
    return 0;
}
