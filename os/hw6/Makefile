CC = gcc
CFLAGS = -Wall -lpthread
TARGET = a.out
OBJECTS = list.o main.o

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)   

clean:
	rm *.o a.out
