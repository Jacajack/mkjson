CFLAGS = -Wall -Os -I.
CC = gcc
AR = ar

ifneq ($(ASPRINTF),)
CFLAGS += -D_GNU_SOURCE
endif

#Builds object and a static library file
all: clean
	$(CC) $(CFLAGS) -c mkjson.c -o mkjson.o
	$(AR) -cvq libmkjson.a mkjson.o
	$(AR) -t libmkjson.a

#Normal cleanup
clean:
	-rm mkjson.o
	-rm libmkjson.a
	-rm example

#Build the example snippet	
example: all
	gcc -o example examples/example.c -I. -L. -lmkjson

	
