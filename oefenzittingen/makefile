HOME_EXERCISE = /localhost/packages/tws
CXXFLAGS =    -DGLAS_COMPLEX       -I$(HOME_EXERCISE)/soft/boost_sandbox -I$(HOME_EXERCISE)/soft/glas2 
LDFLAGS =     
LIBS =   -L$(HOME_EXERCISE)/soft/glas2/libs/ -lglas2 -lstdc++



example.o : example.cpp $(EXAMPLE_DEPS)
	@g++ -c -std=c++11 -Wall   -O3 -fprefetch-loop-arrays -funroll-loops -ffast-math -DNDEBUG $(CXXFLAGS) -o example.o example.cpp


example: example.o
	@g++ -o example $(LDFLAGS) example.o $(LIBS)