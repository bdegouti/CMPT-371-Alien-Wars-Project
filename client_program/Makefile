CC = gcc
CFLAGS = -g -Wall -Werror

all: 	client
		
client: 	client.c
				$(CC) $(CFLAGS) -o client client.c userAction.c playerGameInstructions.c clientServerAPI.c

clean:
	$(RM) *.o client