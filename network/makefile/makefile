SOURCE=$(wildcard *.c)
OBJECT=$(patsubst %.c,%.o,$(SOURCE))
TARGET=main
CC=gcc
CPPFLAGS=-I./

$(TARGET):$(OBJECT)
	$(CC) -o $@ $^ 

%.o:%.c
	$(CC) -g -c $< $(CPPFLAGS)
.PHONY:clean
clean:
	rm -f $(TARGET) $(OBJECT)
