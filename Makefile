CC = gcc
CFLAGS = -g -Wall
TARGET = main
HEADERS = defs.h
LIB = -lpari -lpthread -lm
BENCH = benchmark

all: $(TARGET)
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LIB) -fopenmp

cppversion:
	$(CC) -c -o capis.o capis.c $(LIB)
	g++ -g -O2 -pthread -std=c++11 $(TARGET).cpp -o $(TARGET) capis.o -lpari -I$(HOME)/sw/include -L$(HOME)/sw/lib -lntl -lgf2x -lgmp

clean:
	rm $(TARGET) $(BENCH)

benchmark:
	g++ $(CFLAGS) -o $(BENCH) $(BENCH).c -lgmp
