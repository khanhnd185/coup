#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "coup.h"
#include "interface.h"
#include "host.h"

int main() {
    char str[20];

    srand(time(NULL));
    printf("Input number of players: ");
    scanf("%[^\n]%*c", str);
    int num_players = atoi(str);
    
    Player **ppplayers = malloc(sizeof(Player *) * num_players);
    Host *phost = malloc(sizeof(Host));
    phost->num_players = num_players;
    phost->players = ppplayers;



    for (int i = 0; i < num_players; i++) {
        ppplayers[i] = malloc(sizeof(Player));
        ask_player_name(phost, i);

        ppplayers[i]->coins = 2;
        ppplayers[i]->num_influences = 2;
        ppplayers[i]->influences[0] = rand() % enNumRole;
        ppplayers[i]->influences[1] = rand() % enNumRole;
        printf("Player %d [%s][%s]\n\n"
                , i
                , gRoleString[ppplayers[i]->influences[0]]
                , gRoleString[ppplayers[i]->influences[1]]);
    }

    run(phost);

    for (int i = 0; i < num_players; i++) {
        free(ppplayers[i]);
    } 
    free(ppplayers);
}
