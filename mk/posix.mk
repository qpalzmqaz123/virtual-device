# posix
CC = gcc
AR = ar

CFLAGS = -g -Wall -O3

DEFINES =

CFLAGS += $(shell sdl2-config --cflags)
LIBS += $(shell sdl2-config --libs)
LIBS += -lpthread

