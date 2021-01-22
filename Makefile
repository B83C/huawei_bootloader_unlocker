CC = gcc
CFLAGS= -I. -O2

SRC=$(wildcard *.c)

OUTPUT=./unlock

all: ${OUTPUT}

${OUTPUT}: ${SRC}
	${CC} ${CFLAGS} $^ -o ${OUTPUT}

.PHONY: all
	
