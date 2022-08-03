# CMPT-371-Alien-Wars-Project  

## Development Notes 
- Be sure to pull the repo before you make changes. Try to prevent merge conflicts as much as possible. 
- compile in linux using gcc. 
- Delete your output files! 
- Use Camel Case 
- Avoid using typedef unless impossible otherwise. 
- 4 spaces, come on guys. 


## Client Overview
CLIENT Connect to Server Function   

Client Input   
- CLIENT INPUT Function  
- CLIENT INPUT TO GAME API Function   
- Client send Game Input API to Server   

Client Output    
- Client Recieves from Server   
- Client Display Server Res   

Client Takedown   

## Server Overview 

Server Start
- Server Connects to 4 players

Game starts 
- Server Init Player Connection (Makes child for each player, listens for action req's) 
- Server Init Game State (Makes queues and player stats)

while game not over  
- listen to player sockets, add player action req to player queues
- each 5 sec, take players action and execute, then broadcast result to players 

game over 
- broadcast results to players 
- close connections 
