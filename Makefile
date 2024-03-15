CC = gcc
CFLAGS  = -g -Wall
TARGET = simulator
SRC = src

all: $(TARGET) clean

$(TARGET):  $(SRC)/sim.c $(SRC)/switch.c $(SRC)/switch.h $(SRC)/common.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)/sim.c $(SRC)/switch.c

clean:
	$(RM) *.o $(SRC)/*.o *~ $(SRC)/*.gch