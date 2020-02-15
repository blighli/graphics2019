CC = g++ 
VERSION = -g -std=c++11 -Wall
INCLUDE = -I../include -lglut -lGL
TARGET = ../bin/Pacman
SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %cpp,%o,$(SRCS))

all:$(TARGET)
	
$(TARGET):$(OBJS)
	$(CC) $(OBJS) $(VERSION) $(INCLUDE) -o $(TARGET)

%.o:%.cpp
	$(CC) $(VERSION) $(INCLUDE) -c $<

.PHONY:clean
clean:
	rm -rf $(OBJS) $(TARGET)
