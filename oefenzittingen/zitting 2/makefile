HOME_EXERCISE = /localhost/packages/tws
CXXFLAGS =    -DGLAS_COMPLEX       -I$(HOME_EXERCISE)/soft/boost_sandbox -I$(HOME_EXERCISE)/soft/glas2 
LDFLAGS =     -std=c++11
LIBS =   -L$(HOME_EXERCISE)/soft/glas2/libs/ -lglas2 -lstdc++



test1: test_cg.o vector.o vector_sum.o #cg.o
	@g++ -o test1 $(LDFLAGS) test_cg.cpp # vector.o vector_sum.o #cg.o $(LIBS)

test_cg.o : test_cg.cpp $(EXAMPLE_DEPS)
	@g++ -c -std=c++11 -Wall    -fprefetch-loop-arrays -funroll-loops -ffast-math -DNDEBUG $(CXXFLAGS)  test_cg.cpp

vector.o: vector.hpp
	@g++ -c vector.hpp $(LDFLAGS) $(LIBS)
	
vector_sum.o: vector_sum.hpp
	@g++  -c vector_sum.hpp $(LDFLAGS) $(LIBS)

cg.o: cg.hpp
	@g++  -c cg.hpp $(LDFLAGS) $(LIBS)


