CC = g++
CFLAGS = -g -Wall

memcon: main.o FileHandler.o CMDHandler.o
	$(CC) $(CFLAGS) -o memcon main.o FileHandler.o CMDHandler.o

clean: 
	$(RM) *.o memcon







