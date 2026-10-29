CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2
SOURCES = main.cpp hospital.cpp storage.cpp

all: smartcare

smartcare: $(SOURCES) hospital.h storage.h models.h
	$(CXX) $(CXXFLAGS) $(SOURCES) -o smartcare

clean:
	rm -f smartcare
