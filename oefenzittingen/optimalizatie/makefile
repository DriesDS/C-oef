HOME_EXERCISE = /localhost/packages/tws
CXXFLAGS =    -DGLAS_COMPLEX       -I$(HOME_EXERCISE)/soft/boost_sandbox -I$(HOME_EXERCISE)/soft/glas2 
LDFLAGS =     
LIBS =   -L$(HOME_EXERCISE)/soft/glas2/libs/ -lglas2 -lstdc++



test1: test_cg.o vector.o vector_sum.o cg.o
	@g++ -o test1 $(LDFLAGS) test_cg.o vector.o vector_sum.o cg.o $(LIBS)

test_cg.o : test_cg.cpp $(EXAMPLE_DEPS)
	@g++ -o test_cg.o -c -std=c++11 -Wall   -O3 -fprefetch-loop-arrays -funroll-loops -ffast-math -DNDEBUG $(CXXFLAGS)  test_cg.cpp

vector.o: vector.hpp
	@g++ -o vector.o -c vector.hpp $(LDFLAGS) $(LIBS)
	
vector_sum.o: vector_cum.hpp
	@g++ -o vector_sum.o -c vector_sum.hpp $(LDFLAGS) $(LIBS)

cg.o: cg.hpp
	@g++ -o cg.o -c cg.hpp $(LDFLAGS) $(LIBS)


