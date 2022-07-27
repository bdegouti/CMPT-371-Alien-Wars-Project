# CMPT-371-Alien-Wars-Project

# SERVER CODE 
start_server()
while(connected sockets != 4) 
    listen_for_socket_connections() # Make a child process for each player.

init_gamestate()
while(game_not_over)
    listen_for_actions() and play_game() 

game_over() 
close_connections()


# listen_for_actions() 
for each child in player processes
    child.listen() 
    process_client_req()
    execute_client_req() #adding valid actions to player action queue


# play_game()
after each 5 seconds while game not over 
    for player in players
        action = player.q.pop() 
        execute(player, action) 
    broadcast_gamestate() 
