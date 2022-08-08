CC = gcc
CFLAGS = -g -Wall -Werror

all: 	client
		
client: 	client.c
				$(CC) $(CFLAGS) -o client client.c userInput.c

clean:
	$(RM) *.o client