ifdef OPT
  CC = clang++
  CCFLAGS = -std=c++11 -O2 -g -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic
else
  CC=g++
  CCFLAGS = -std=c++11 -O2 -g -Wall -Wextra -pedantic
endif

HEADERS=formula.h clause.h clause_impl.h formula_impl.h solver.h
SOURCES=sat.cpp formula.cpp clause.cpp clause_impl.cpp formula_impl.cpp solver.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=sat

all: $(SOURCES) $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@

%.o: %.cpp $(HEADERS)  
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm -f sat *.o
