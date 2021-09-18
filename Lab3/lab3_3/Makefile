CC = g++
TARGET = main
OBJ = main.o operation.o

CFLAGS = -c -Wall

$(TARGET) : $(OBJ)
	$(CC) -o $@ $(OBJ)

%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(TARGET)

.PHONY: clean
