CC=g++
EXEC = test.out
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
LDFLAGS = -lpthread

all: $(EXEC)

test: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ 
	
%.o : %.cpp
	$(CC) -c $^
	
clean :
	rm *.o
