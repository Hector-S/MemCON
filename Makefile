CC = g++
CFLAGS = -g -Wall

memcon: main.o FileHandler.o CMDHandler.o Queue.o 
	$(CC) $(CFLAGS) -o memcon main.o FileHandler.o CMDHandler.o Queue.o

clean: 
	$(RM) *.o memcon







