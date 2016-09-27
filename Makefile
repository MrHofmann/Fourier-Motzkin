PROGRAM	= FM
CXX	= g++
CFLAGS	= -std=c++11 -Wall

PROGRAM: main.o relation.o fm.o
	$(CXX) -o $(PROGRAM) $(CFLAGS) main.o relation.o fm.o

.PHONY: clean

clean:
	rm -f *.o $(PROGRAM)
