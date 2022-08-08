// THIS IS ALL PSUDO CODE!

bool start_server() {
    server_socket = init_server_socket();
    while(server_connections != 4) {
        server_socket.listen();
        new_player = server_socket.accept();
        init_new_player(new_player);
        new_player.send("you are player x!"); 
    }
    return true; // if no errors 
}

struct Game {
    bool gameover;
    array[string] player0_queue;
    array[string] player1_queue;
    array[string] player2_queue;
    array[string] player3_queue;
    array[int] health;
    array[int] gun; 
    bool gunlocked;
}

Game init_gamestate() {
    Game gamestate = {
        gameover = false,
        player0_queue = [],
        player1_queue = [],
        player2_queue = [],
        player3_queue = [],
        health = [100, 100, 100, 100],
        gun = [0, 0, 0, 0],
        gunlocked = false,
    }
    return gamestate;
}

init_player_monitor() {
    while(infin)
        player0.listen()
        player1.listen()
        player2.listen()
        player3.listen()
        for player in players
            player.listen()
            player_action = player.accept(); // We must translate alien_wars_api req into player action
            gamestate.playerqueue.append(player_action);
        
}

listen_for_actions() {
    for each child in player processes
        child.listen() 
        process_client_req()
        execute_client_req() #adding valid actions to player action queue
}

play_game() {
    after each 5 seconds while game not over 
        for player in players
            action = player.q.pop() 
            execute(player, action) 
        broadcast_gamestate() 
}


int main() {
    start_server();  

    Game gamestate = init_gamestate(); 
    init_player_monitor(); 
    while(game_not_over)  
        listen_for_actions() and play_game() 

    game_over() 
    close_connections()
}

