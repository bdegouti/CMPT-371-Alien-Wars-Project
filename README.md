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


## Development Notes 
- Be sure to pull the repo before you make changes. Try to prevent merge conflicts as much as possidble.
- compile in linux using gcc.
- Delete your output files! 
- Use camelCase.
- Avoid using typedef unless impossible otherwise. 
- 4 spaces.