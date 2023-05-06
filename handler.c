#include <stdio.h>
#include <stdlib.h>
#include "handler.h"
#include "msg.h"

void print_player_info(Player *player)
{
    printf("[%s] ", player->name);
    for (char i = 0; i < player->num_influences; i++) {
        printf("[%s]", gRoleString[player->influences[i]]);
    }
    printf("[%d]: ", player->coins);
}

static char handle_ask_player_action(struct MsgChooseAction *msg, Player **players, char num_players, char p)
{
    char str[8];

    print_player_info(players[p]);

    for (char i = 0; i < enNumAction; i++) {
        printf("[%d:%s] ", i, gActionString[i]);
    }
    printf(": ");

    scanf("%[^\n]%*c", str);
    char j = (char) atoi(str);

    if (j >= enNumAction) {
        return enIncome;
    }

    return j;
}

static char handle_ask_player_object(struct MsgChooseObject *msg, Player **players, char num_players, char p)
{
    char j, str[8];

    print_player_info(players[p]);
    printf("do %s with: ", gActionString[msg->action]);
    for (char i  = 0; i < msg->num_objects; i++) {
        j = msg->players[i];
        printf("[%d:%s] ", j, players[j]->name);
    }
    printf(": ");

    scanf("%[^\n]%*c", str);
    char k = (char) atoi(str);

    return k;
}

static char handle_ask_player_reveal(struct MsgRevealInfluence *msg, Player **players, char num_players, char p)
{
    char str[8];

    print_player_info(players[p]);
    printf("choose influence to reveal: ");
    for (char i = 0; i < msg->num_influences; i++) {
        printf("[%d:%s] ", i, gRoleString[msg->influences[i]]);
    }
    printf(": ");

    scanf("%[^\n]%*c", str);
    char j = (char) atoi(str);

    return j;
}

static char handle_ask_player_remove(struct MsgRemoveInfluence *msg, Player **players, char num_players, char p)
{
    char str[8];

    print_player_info(players[p]);
    printf("choose influence to reveal: ");
    for (char i = 0; i < msg->num_influences; i++) {
        printf("[%d:%s] ", i, gRoleString[msg->influences[i]]);
    }
    printf(": ");

    scanf("%[^\n]%*c", str);
    char j = (char) atoi(str);

    return j;
}

static char handle_ask_player_counter(struct MsgChooseCounter *msg, Player **players, char num_players, char p)
{
    char str[8];

    print_player_info(players[p]);
    printf("Player %s want to do %s "
            , players[msg->subject]->name
            , gActionString[msg->action]);

    if (gActionObject[msg->action]) {
        printf("with player %s ", players[msg->object]->name); 
    }
    printf(": ");
    printf("[0:Pass] [1:Challenge] ");

    if (gBlockAction[msg->action]) {
        printf("[2:Block] : ");
    }
    else {
        printf(": ");
    }

    scanf("%[^\n]%*c", str);
    char j = (char) atoi(str);

    return j;
}

static char handle_ask_player_accept_challenge(struct MsgAcceptChallenge *msg, Player **players, char num_players, char p)
{
    char str[8];

    print_player_info(players[p]);
    printf("challenged by player %s : ", players[msg->challenger]->name);
    printf("[0:Refuse] [1:Accept] : ");

    scanf("%[^\n]%*c", str);
    char j = (char) atoi(str);

    return j;    
}

static char handle_ask_player_challenge(struct MsgChallenge *msg, Player **players, char num_players, char p)
{
    char str[8];

    print_player_info(players[p]);
    printf("challenge or refuse %s : ", players[msg->object]->name);
    printf("[0:Refuse] [1:Challenge] : ");

    scanf("%[^\n]%*c", str);
    char j = (char) atoi(str);

    return j;
}

static char handle_ask_player_block_by(struct MsgBlockByWhom *msg, Player **players, char num_players, char p)
{
    char str[8];
    char *role_lists = gRoleCounterMatrix[msg->action];

    print_player_info(players[p]);
    printf("block %s by: ", gActionString[msg->action]);
    for (char i = 0; i < msg->num_roles; i++) {
        printf("[%d:%s] ", msg->roles[i], gRoleString[msg->roles[i]]);
    }

    scanf("%[^\n]%*c", str);
    char j = (char) atoi(str);

    return j;
}

static char handle_ask_player_choose_role(struct MsgChooseRole *msg, Player **players, char num_players, char p)
{
    char str[8];
    print_player_info(players[p]);
    printf("Choose: ");

    for (char i = 0; i < msg->num_roles; i++) {
        printf("[%d:%s] ", i, gRoleString[msg->roles[i]]);
    }

    scanf("%[^\n]%*c", str);
    char j = (char) atoi(str);

    return j;
}


char handle_message(char *m, Player **players, char num_players, char p)
{
    char rep;
    struct MsgWin *msg;

    switch (m[0]) {
        case enWin:
            msg = (struct MsgWin *) m;
            printf("Winner is %d\n.", msg->winner);
            rep = 0;
            break;
        case enChooseAction:
            rep = handle_ask_player_action((struct MsgChooseAction *) m, players, num_players, p);
            break;
        case enChooseObject:
            rep = handle_ask_player_object((struct MsgChooseObject *) m, players, num_players, p);
            break;
        case enRevealInfluence:
            rep = handle_ask_player_reveal((struct MsgRevealInfluence *) m, players, num_players, p);
            break;
        case enRemoveInfluence:
            rep = handle_ask_player_remove((struct MsgRemoveInfluence *) m, players, num_players, p);
            break;
        case enChooseCounter:
            rep = handle_ask_player_counter((struct MsgChooseCounter *) m, players, num_players, p);
            break;
        case enAcceptChallenge:
            rep = handle_ask_player_accept_challenge((struct MsgAcceptChallenge *) m, players, num_players, p);
            break;
        case enChallengePlayer:
            rep = handle_ask_player_challenge((struct MsgChallenge *) m, players, num_players, p);
            break;
        case enBlockByWhom:
            rep = handle_ask_player_block_by((struct MsgBlockByWhom *) m, players, num_players, p);
            break;
        case enChooseRole:
            rep = handle_ask_player_choose_role((struct MsgChooseRole *) m, players, num_players, p);
            break;
    }
    return rep;
}

