CC=g++
EXEC = test
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
LDFLAGS = -lpthread -lm -lwiringPi

all: $(EXEC)

test: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ 
	
%.o : %.cpp
	$(CC) -c $^
	
clean :
	rm *.o
