CC = gcc
CFLAGS = -g -Wall -Werror
#-lpthread
LDFLAGS =
OBJFILES = client.o clientBackend.o clientGUI.o clientServerParser.o clientServerAPI.o playerGameInstructions.o userAction.o ../server_program/gameStructures.o
TARGET = client

all: 	$(TARGET)
		
$(TARGET): 	$(OBJFILES)
		$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	$(RM) *.o $(TARGET) ../server_program/gameStructures.o