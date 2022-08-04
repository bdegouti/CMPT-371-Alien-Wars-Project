all: client tempServer

client: client.c
	gcc -g -Wall -o client client.c clientBackend.c userInput.c

tempServer: tempServer.c
	gcc -g -Wall -o tempServer tempServer.c

clean:
	rm -rf client tempServer