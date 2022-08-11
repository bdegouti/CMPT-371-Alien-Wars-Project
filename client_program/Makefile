CC = gcc
CFLAGS = -g -Wall -Werror
LDFLAGS = -lpthread
OBJFILES = client.o clientBackend.o clientGUI.o clientServerParser.o clientServerAPI.o playerGameInstructions.o userAction.o ../server_program/gameStructures.o
TARGET = client

all: 	$(TARGET)
		
$(TARGET): 	$(OBJFILES)
		$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
		$(MAKE) obj_clean
clean:
	$(RM) *.o $(TARGET) ../server_program/gameStructures.o

obj_clean:
	$(RM) *.o ../server_program/gameStructures.o