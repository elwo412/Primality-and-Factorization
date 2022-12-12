CC = gcc
CFLAGS = -g -Wall
TARGET = main
HEADERS = defs.h
LIB = -lpari -lpthread
BENCH = benchmark

all: $(TARGET)
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LIB) -fopenmp

clean:
	rm $(TARGET) $(BENCH)

benchmark:
	g++ $(CFLAGS) -o $(BENCH) $(BENCH).c -lgmp
