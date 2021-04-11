CC = gcc
CXX = g++

CFLAGS = -g 
OBJS = namelookup.o

TARGET = namelookup

$(TARGET) : $(OBJS)
	gcc $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS)
	
.PHONY : clean
clean :
	rm -rf $(OBJS) $(TARGET)
