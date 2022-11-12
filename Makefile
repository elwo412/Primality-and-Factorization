CC = g++
CFLAGS = -g -Wall
TARGET = main
HEADERS = defs.h
LIB = -lpari
BENCH = benchmark

all: $(TARGET)
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LIB)

clean:
	rm $(TARGET) $(BENCH)

benchmark:
	$(CC) $(CFLAGS) -o $(BENCH) $(BENCH).c -lgmp
