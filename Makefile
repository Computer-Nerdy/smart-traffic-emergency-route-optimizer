# Makefile for Smart Traffic & Emergency Route Optimization System
# Course: 21CSC201J - Data Structures and Algorithms
# SRM Institute of Science and Technology, Ramapuram

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11 -Iinclude
SRCS = src/main.c src/min_heap.c src/city_graph.c src/dijkstra.c src/iot_sensor.c src/cli.c
OBJS = $(SRCS:.c=.o)
TARGET = smart_traffic.exe

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

test: $(TARGET)
	./$(TARGET) --test

demo: $(TARGET)
	./$(TARGET) --demo

clean:
	rm -f $(TARGET) src/*.o

.PHONY: all test demo clean
