CC = gcc
CFLAGS = -Wall
TARGET = a.out
OBJECTS = list.o main.o

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFALGS) -o $@ $^

clean:
	rm *.o a.out
