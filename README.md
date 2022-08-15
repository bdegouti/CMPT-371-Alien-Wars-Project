# CMPT 371 Alien Wars Project  

## Project Proposal: Alien Wars
Alien Wars is a 2 v 2 real time strategy game. The goal of the game is for your team to kill the other team. 

Actions 
- Shoot: Fire at a single enemy. This deals 1 damage. 
- Defend: Heals you or your teammate 1 health. 
- Gun: Loads your gun which, at 100 will upgrade, which will deal 2x damage for each shoot you do, for 10 rounds. 
This gun can only be upgraded by one person at a time, so whoever reaches 100 loads first will recieve the upgrade,
and all other players can no longer upgrade their gun. 

## Game Design
Client Server Connection: The clients connect to the server. Once 4 clients have connected, the server will send out
which clients are on the same teams, and start the game. 

Client: The client will be asked for Actions, which are sent to the server. The client will recieve the current gamestate, 
or a game over gamestate. 

Server: The server will listen to 4 clients and send their actions to the Server GameLogic. 
The server will take Server GameLogic gamestate and send it to the 4 clients. 

Server GameLogic: The Server GameLogic takes player actions and adds them to the players action queues. 
Each round, the Server GameLogic executes the first action from each players queue. Then it returns the current gamestate.

Client Server Close Connection: When the GameLogic sends a game over gamestate to the server, 
the server will send the game over gamestate to the 4 clients, and then the server will close. 
When a client recieves a game over gamestate, it will close its connection. 

## Application Layer Messaging Scheme
Our Application sends messages through converting game objects into text, using sockets to send and recieve the message, 
and then sends the text to a converter which creates game objects for the reciever. 
For example, a gamestate is converted from various objects which hold gamestate data such as player queues into a single string,
which starts off discribing the gamestate, then listing specific data. This super string is then sent with sockets to the reciever, who takes a string and converts it into game objects.

In the tcpserver.c sockets are setup as following:
![sockets1](https://user-images.githubusercontent.com/71346386/184566355-aeb5712b-5495-452b-bb9b-7c61e25dcae3.png)

and then bound to the port

![sockets2](https://user-images.githubusercontent.com/71346386/184566371-c6b76d7b-c613-420a-adc9-ddcff6d8e77b.png)
allowing a client to connect by any of the servers interfaces.

the server will then listen until a client connects, accepting 4 clients in total. the newly opened sockets will be added to a pollfd structure. 

![sockets3](https://user-images.githubusercontent.com/71346386/184566613-ae2d5211-0a95-431a-80af-2673c8c4b69d.png)

this structure will then use the poll system call to moniter for events from any of the clients, and recv the incoming messages to parse and pass to the GameLogic (represented by gameStructure).

the shared object is handled in the server backend (gameStructure.c), where control over the gun is determined by whichever player enters five gun actions--control will then last for 5 "bullets", doubling the damage of each attack.

![gun1](https://user-images.githubusercontent.com/71346386/184567559-9603308f-65eb-486d-88ad-62ce7261bdf0.png)

![gun2](https://user-images.githubusercontent.com/71346386/184567572-2a887021-c7ee-41d1-96bd-9dafde55027e.png)

## Group Members and their % contribution
Ben de Goutiere - 20% (Server Frontend, GameLogic)
Kerem Sarisen - 20%   (Game Logic)
Mike Ostrowka - 20%   (Client Frontend, Client Backend)
Andy Wang - 20%       (Socket setup)
Yosup Cheon - 20%     (Client Frontend, Client Backend)
