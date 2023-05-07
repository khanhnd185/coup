#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "handler.h"
#include "msg.h"

static char handle_ask_player_action(struct MsgChooseAction *msg, Player **players, char num_players, char p)
{
    char str[8];

    print_player_private_info(players[p]);

    printf(": ");
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

    print_player_private_info(players[p]);
    printf(": do %s with: ", gActionString[msg->action]);
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

    print_player_private_info(players[p]);
    printf(": choose influence to reveal: ");
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

    print_player_private_info(players[p]);
    printf(": choose influence to remove: ");
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

    print_player_private_info(players[p]);
    printf(": Player %s want to do %s "
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

    print_player_private_info(players[p]);
    printf(": challenged by player %s : ", players[msg->challenger]->name);
    printf("[0:Refuse] [1:Accept] : ");

    scanf("%[^\n]%*c", str);
    char j = (char) atoi(str);

    return j;    
}

static char handle_ask_player_challenge(struct MsgChallenge *msg, Player **players, char num_players, char p)
{
    char str[8];

    print_player_private_info(players[p]);
    printf(": challenge or refuse %s : ", players[msg->object]->name);
    printf("[0:Refuse] [1:Challenge] : ");

    scanf("%[^\n]%*c", str);
    char j = (char) atoi(str);

    return j;
}

static char handle_ask_player_block_by(struct MsgBlockByWhom *msg, Player **players, char num_players, char p)
{
    char str[8];
    char *role_lists = gRoleCounterMatrix[msg->action];

    print_player_private_info(players[p]);
    printf(": block %s by: ", gActionString[msg->action]);
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
    print_player_private_info(players[p]);
    printf(": Choose: ");

    for (char i = 0; i < msg->num_roles; i++) {
        printf("[%d:%s] ", i, gRoleString[msg->roles[i]]);
    }

    scanf("%[^\n]%*c", str);
    char j = (char) atoi(str);

    return j;
}

static char handle_update_player_info(struct MsgName *msg, Player **players, char num_players, char p)
{
    char i = msg->number;

    players[i]->coins = msg->coins;
    players[i]->num_influences = msg->numinf;
    players[i]->influences[0] = msg->role[0];
    players[i]->influences[1] = msg->role[1];

    if (i == p) {
        print_player_private_info(players[i]);
        printf("\n");
    }
    else {
        print_player_public_info(players[i]);
        printf("\n");
    }

    return 0;
}

static char handle_notify_win(struct MsgWin *msg, Player **players, char num_players, char p)
{
    printf("Winner is %s\n.", players[msg->winner]->name);
    return 0;
}

static char handle_msg_log(int sockfd)
{
    char str[80];
    bzero(str, 80);

    read(sockfd, str, 80);
    printf("%s", str);

    return 0;
}

char handle_message(int sockfd, char *m, Player **players, char num_players, char p)
{
    char rep;


    switch (m[0]) {
        case enName:
            rep = handle_update_player_info((struct MsgName *) m, players, num_players, p);
            break;
        case enMsgLog:
            rep = handle_msg_log(sockfd);
            break;
        case enWin:
            rep = handle_notify_win((struct MsgWin *) m, players, num_players, p);
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

