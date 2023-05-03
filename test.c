#include <stdio.h>
#include <stdlib.h>

#include "coup.h"

int main() {
    char str[20];

    printf("Input number of players: ");
    scanf("%[^\n]%*c", str);
    int num_players = atoi(str);
    
    Player **ppplayers = malloc(sizeof(Player *) * num_players);


    for (int i = 0; i < num_players; i++) {
        ppplayers[i] = malloc(sizeof(Player));
        printf("Input name of player %d: ", i);
        scanf("%[^\n]%*c", ppplayers[i]->name);
    }


    for (int i = 0; i < num_players; i++) {
        printf("Name of player %d: %s \n", i, ppplayers[i]->name);
        free(ppplayers[i]);
    }


    free(ppplayers);
}
