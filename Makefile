SRC = UnixLs.c
TARGET = UnixLs
CC = gcc
CFLAGS = -Wall
$(TARGET): UnixLs.o
	$(CC) $^ -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o
	rm $(TARGET)

