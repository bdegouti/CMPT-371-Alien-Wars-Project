CC = gcc
CFLAGS = -g -Wall -Werror
LDFLAGS = -pthread
OBJFILES = tcpserver.o gameStructures.o
TARGET = tcpServer

all: 	$(TARGET)
		
$(TARGET): 	$(OBJFILES)
		$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
		$(MAKE) obj_clean
clean:
	$(RM) *.o $(TARGET)

obj_clean:
	$(RM) *.o