#include "interface.h"
#include <stdio.h>
#include <stdlib.h>

void ask_player_name(Host *phost, unsigned char i)
{
    printf("Input name of player %d: ", i+1);
    scanf("%[^\n]%*c", phost->players[i]->name);    
}

Action ask_player_action(Host *phost, unsigned char p)
{
    char str[8];
    Player *player = phost->players[p];

    printf("[%s] ", player->name);
    for (unsigned char i = 0; i < player->num_influences; i++) {\
        printf("[%s]", gRoleString[player->influences[i]]);
    }
    printf("[%d]: ", player->coins);

    for (unsigned char i = 0; i < enNumAction; i++) {
        printf("[%d:%s] ", i, gActionString[i]);
    }
    printf(": ");

    scanf("%[^\n]%*c", str);
    unsigned char j = (unsigned char) atoi(str);

    if (j >= enNumAction) {
        return enIncome;
    }

    printf("[Log] Player %s want to do %s\n", phost->players[p]->name, gActionString[j]);
    return j;
}

unsigned char ask_player_object(Host *phost, unsigned char who_ask, unsigned char action)
{
    char str[8];
    unsigned char default_player = who_ask;
    unsigned char j;

    printf("[%s] do %s with: ", phost->players[who_ask]->name, gActionString[action]);
    for (unsigned char i  = (who_ask+1); i < (phost->num_players + who_ask); i++) {
        j = i % phost->num_players;

        if (phost->players[j]->num_influences == 0) {
            continue;
        }

        if (default_player == who_ask) {
            default_player = j;
        }

        printf("[%d:%s] ", j, phost->players[j]->name);
    }
    printf(": ");

    scanf("%[^\n]%*c", str);
    unsigned char k = (unsigned char) atoi(str);

    if (k >= phost->num_players) {
        k = default_player;
    }

    printf("[Log] Player %s want to do %s with %s\n"
            , phost->players[who_ask]->name
            , gActionString[action]
            , phost->players[k]->name);
    return k;
}

unsigned char ask_player_remove(Host *phost, unsigned char p)
{
    char str[8];
    printf("[%s] choose influence(s) to remove: ", phost->players[p]->name);
    for (unsigned char i = 0; i < phost->players[p]->num_influences; i++) {
        printf("[%d:%s] ", i, gRoleString[phost->players[p]->influences[i]]);
    }
    printf(": ");

    scanf("%[^\n]%*c", str);
    unsigned char j = (unsigned char) atoi(str);

    if (j >= phost->players[p]->num_influences) {
        j = 0;
    }

    printf("[Log] Player %s remove %s\n"
            , phost->players[p]->name
            , gRoleString[phost->players[p]->influences[j]]);
    return j;
}

void notify_player_message(Host *phost, unsigned char p, char *msg)
{
    printf("[Log] Player %s: %s\n", phost->players[p]->name, msg);
}

void notify_player_take_action(Host *phost, unsigned char subject, Action action, unsigned char object)
{
    printf("[Log] Player %s did %s "
            , phost->players[subject]->name
            , gActionString[action]);
    if (gActionObject[action]) {
        printf("with player %s\n", phost->players[object]->name); 
    }
    else {
        printf("\n");
    }
}


Counter ask_player_counter(Host *phost, unsigned char actor, Action action, unsigned char object, unsigned char counter)
{
    char str[8];

    printf("[%s] Player %s want to do %s "
            , phost->players[counter]->name
            , phost->players[actor]->name
            , gActionString[action]);

    if (gActionObject[action]) {
        printf("with player %s ", phost->players[object]->name); 
    }
    printf(": ");
    printf("[0:Pass] [1:Challenge] ");

    if (gBlockAction[action]) {
        printf("[2:Block] : ");
    }
    else {
        printf(": ");
    }

    scanf("%[^\n]%*c", str);
    unsigned char j = (unsigned char) atoi(str);

    if (j >= enNumCounter) {
        j = enPass;
    }

    printf("[Log] Player %s %s player %s doing %s "
                , phost->players[counter]->name
                , gCounterString[j]
                , phost->players[actor]->name
                , gActionString[action]);
    if (gActionObject[action]) {
        printf("with player %s\n", phost->players[object]->name); 
    }
    else {
        printf("\n");
    }
    return j;
}

unsigned char ask_player_accept_challenge(Host *phost, unsigned char answerer, unsigned char challenger)
{
    char str[8];
    printf("[%s] challenged by player %s : ", phost->players[answerer]->name, phost->players[challenger]->name);
    printf("[0:Refuse] [1:Accept] : ");

    scanf("%[^\n]%*c", str);
    unsigned char j = (unsigned char) atoi(str);

    if (j > 1) {
        j = 1;
    }

    printf("[Log] Player %s %s player %s challenge\n"
                , phost->players[answerer]->name
                , (j == 0) ? "Refuse" : "Accept"
                , phost->players[challenger]->name);
    return j;    
}

void notify_winner(Host *phost)
{
    for (unsigned char i = 0; i < phost->num_players; i++) {
        if (phost->players[i]->num_influences > 0) {
            printf("Winner is %s\n", phost->players[i]->name);
            return;
        }
    }

    printf("No one wins?\n");
}

Counter ask_player_challenge(Host *phost, unsigned char object, unsigned char challenger)
{
    char str[8];
    printf("[%s] challenge or refuse %s : ", phost->players[challenger]->name, phost->players[object]->name);
    printf("[0:Refuse] [1:Challenge] : ");

    scanf("%[^\n]%*c", str);
    unsigned char j = (unsigned char) atoi(str);

    if (j > enChallenge) {
        j = enPass;
    }

    printf("[Log] Player %s %s %s\n"
            , phost->players[challenger]->name
            , gCounterString[j]
            , phost->players[object]->name);
    return j;
}

// Revise this function
Role ask_player_block_by(Host *phost, unsigned char answerer, Action blocked_action)
{
    char str[8];
    printf("[%s] block %s by: ", phost->players[answerer]->name, gActionString[blocked_action]);
    
    if (blocked_action == enSteal) {
        printf("[2:Ambassador] [3:Captain] : ");
    }
    else if (blocked_action == enForeignAid) {
        printf("[0:Duke] : ");
    }
    else if (blocked_action == enAssassinate) {
        printf("[4:Contessa] : ");
    }
    else {
        printf("Something goes wrong : ");
    }

    scanf("%[^\n]%*c", str);
    unsigned char j = (unsigned char) atoi(str);

    if (blocked_action == enSteal) {
        if ((j != enAmbassador) && (j != enCaptain)) {
            j = enAmbassador;
        }
    }
    else if (blocked_action == enForeignAid) {
        j = enDuke;
    }
    else if (blocked_action == enAssassinate) {
        j = enContessa;
    }
    else {
        j = enAmbassador;
    }

    printf("[Log] Player %s block %s by %s\n"
            , phost->players[answerer]->name
            , gActionString[blocked_action]
            , gRoleString[j]);
    return j;

}


unsigned char ask_player_choose_role(Host *phost, unsigned char answerer, Role *roles, unsigned char num_roles)
{
    char str[8];
    printf("[%s] Choose: ", phost->players[answerer]->name);

    for (unsigned char i = 0; i < num_roles; i++) {
        printf("[%d:%s] ", i, gRoleString[roles[i]]);
    }

    scanf("%[^\n]%*c", str);
    unsigned char j = (unsigned char) atoi(str);

    if (j >= num_roles) {
        j = 0;
    }

    printf("[Log] Player %s choose %s\n", phost->players[answerer]->name, gRoleString[roles[j]]);
    return j;
}
