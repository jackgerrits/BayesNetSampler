PLATFORM := $(shell uname)

EXE=sampler$(EXT)

DEFS =-std=c++11 -Wall -Wextra

FILES += $(wildcard *.cpp)
FILES := $(filter-out test.cpp, $(FILES))
FILES := $(filter-out main.cpp, $(FILES))

LINK = $(notdir $(subst .cpp,.o, $(FILES)))

.PHONY:  clean

all : $(EXE)

$(EXE) : main.cpp $(LINK)
	g++ main.cpp $(DEFS) -o $(EXE) $(LINK)

%.o : %.cpp %.h
	g++ $(DEFS) -c $<

testBin: test.cpp $(LINK)
	g++ $(DEFS) -pthread test.cpp $(LINK) libgtest.a -o testBin

test: testBin
	./testBin

clean:
	rm -f *.o $(EXE) testBin
