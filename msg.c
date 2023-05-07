#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "interface.h"
#include "msg.h"

void introduce_players_name(Host *phost, char p)
{
    char j, buff[16];
    struct MsgName msg;

    for (char i  = (p+1); i < (phost->num_players + p); i++) {
        j = i % phost->num_players;

        bzero(&msg, 16);
        msg.opcode = enName;
        msg.number = j;
        strcpy(msg.name, phost->players[j]->name);
        write(phost->fds[p], (char *)&msg, sizeof(struct MsgName));
    }
}

void broadcast_log(Host *phost, char *str)
{
    struct MsgLog msg;

    printf("%s", str);

    bzero(&msg, 16);
    msg.opcode = enMsgLog;

    for (char j = 0; j < phost->num_players; j++) {
        write(phost->fds[j], (char *)&msg, sizeof(struct MsgLog));
        write(phost->fds[j], str, 80);
    }
}

void broadcast_players_info(Host *phost, char i)
{
    struct MsgName msg;

    print_player_private_info(phost->players[i]);
    printf("\n");

    bzero(&msg, 16);
    msg.opcode = enName;
    msg.role[0] = phost->players[i]->influences[0];
    msg.role[1] = phost->players[i]->influences[1];
    msg.number = i;
    msg.coins = phost->players[i]->coins;
    msg.numinf = phost->players[i]->num_influences;

    for (char j = 0; j < phost->num_players; j++) {
        write(phost->fds[j], (char *)&msg, sizeof(struct MsgName));
    }
}

void ask_player_name(Host *phost, char i)
{
    struct MsgName msg;
    struct MsgName rep;

    bzero(&msg, 16);
    bzero(&rep, 16);
    msg.opcode = enName;
    msg.role[0] = phost->players[i]->influences[0];
    msg.role[1] = phost->players[i]->influences[1];
    msg.name[0] = phost->num_players;
    msg.number = i;
    msg.coins = phost->players[i]->coins;
    msg.numinf = phost->players[i]->num_influences;

    write(phost->fds[i], (char *)&msg, sizeof(struct MsgName));
    read(phost->fds[i], (char *)&rep, sizeof(struct MsgName));
    bzero(phost->players[i]->name, 12);
    strcpy(phost->players[i]->name, rep.name);
}

char ask_player_action(Host *phost, char p)
{
    char str[80];
    char buff[16];
    struct MsgChooseAction msg;
    struct MsgReply rep;

    bzero(&msg, 16);
    bzero(&rep, 16);
    msg.opcode = enChooseAction;

    write(phost->fds[p], (char *)&msg, sizeof(struct MsgChooseAction));
    read(phost->fds[p], (char *)&rep, sizeof(struct MsgReply));

    if (rep.reply >= enNumAction) {
        return enIncome;
    }

    sprintf(str, "[Log] Player %s want to do %s\n", phost->players[p]->name, gActionString[rep.reply]);
    broadcast_log(phost, str);
    return rep.reply;
}

char ask_player_object(Host *phost, char p, char action)
{
    char str[80];
    char default_player = p;
    char j, num_objects = 0;
    struct MsgChooseObject msg;
    struct MsgReply rep;

    bzero(&msg, 16);
    bzero(&rep, 16);
    msg.opcode = enChooseObject;
    msg.action = action;

    for (char i  = (p+1); i < (phost->num_players + p); i++) {
        j = i % phost->num_players;

        if (phost->players[j]->num_influences == 0) {
            continue;
        }

        if (default_player == p) {
            default_player = j;
        }

        msg.players[num_objects] = j;
        num_objects += 1;
    }
    msg.num_objects = num_objects;

    write(phost->fds[p], (char *)&msg, sizeof(struct MsgChooseObject));
    read(phost->fds[p], (char *)&rep, sizeof(struct MsgReply));

    if (rep.reply >= phost->num_players) {
        rep.reply = default_player;
    }

    sprintf(str, "[Log] Player %s want to do %s with %s\n"
            , phost->players[p]->name
            , gActionString[action]
            , phost->players[rep.reply]->name);
    broadcast_log(phost, str);
    return rep.reply;
}

char ask_player_reveal(Host *phost, char p)
{
    char str[80];
    char num_objects = 0;
    struct MsgRevealInfluence msg;
    struct MsgReply rep;

    bzero(&msg, 16);
    bzero(&rep, 16);
    msg.opcode = enRevealInfluence;
    msg.num_influences = phost->players[p]->num_influences;
    msg.influences[0] = phost->players[p]->influences[0];
    msg.influences[1] = phost->players[p]->influences[1];

    write(phost->fds[p], (char *)&msg, sizeof(struct MsgRevealInfluence));
    read(phost->fds[p], (char *)&rep, sizeof(struct MsgReply));

    if (rep.reply >= phost->players[p]->num_influences) {
        rep.reply = 0;
    }

    sprintf(str, "[Log] Player %s reveal %s\n"
            , phost->players[p]->name
            , gRoleString[phost->players[p]->influences[rep.reply]]);
    broadcast_log(phost, str);
    return rep.reply;
}

char ask_player_remove(Host *phost, char p)
{
    char str[80];
    char num_objects = 0;
    struct MsgRemoveInfluence msg;
    struct MsgReply rep;

    bzero(&msg, 16);
    bzero(&rep, 16);
    msg.opcode = enRemoveInfluence;
    msg.num_influences = phost->players[p]->num_influences;
    msg.influences[0] = phost->players[p]->influences[0];
    msg.influences[1] = phost->players[p]->influences[1];

    write(phost->fds[p], (char *)&msg, sizeof(struct MsgRemoveInfluence));
    read(phost->fds[p], (char *)&rep, sizeof(struct MsgReply));

    if (rep.reply >= phost->players[p]->num_influences) {
        rep.reply = 0;
    }

    sprintf(str, "[Log] Player %s remove %s\n"
            , phost->players[p]->name
            , gRoleString[phost->players[p]->influences[rep.reply]]);
    broadcast_log(phost, str);
    return rep.reply;
}

void notify_player_message(Host *phost, char p, char *msg)
{
    char str[80];
    sprintf(str, "[Log] Player %s %s\n", phost->players[p]->name, msg);
    broadcast_log(phost, str);
}

void notify_player_take_action(Host *phost, char subject, char action, char object)
{
    char str[80];
    if (gActionObject[action]) {
        sprintf(str, "[Log] Player %s did %s with player %s\n"
                , phost->players[subject]->name
                , gActionString[action]
                , phost->players[object]->name);
    }
    else {
        sprintf(str, "[Log] Player %s did %s\n"
                , phost->players[subject]->name
                , gActionString[action]);
    }
    broadcast_log(phost, str);
}


char ask_player_counter(Host *phost, char actor, char action, char object, char p)
{
    char str[80];
    struct MsgChooseCounter msg;
    struct MsgReply rep;

    bzero(&msg, 16);
    bzero(&rep, 16);
    msg.opcode = enChooseCounter;
    msg.subject = actor;
    msg.action = action;
    msg.object = object;

    write(phost->fds[p], (char *)&msg, sizeof(struct MsgChooseCounter));
    read(phost->fds[p], (char *)&rep, sizeof(struct MsgReply));


    if (rep.reply >= enNumCounter) {
        rep.reply = enPass;
    }

    if (gActionObject[action]) {
        sprintf(str, "[Log] Player %s %s player %s doing %s with player %s\n"
                    , phost->players[p]->name
                    , gCounterString[rep.reply]
                    , phost->players[actor]->name
                    , gActionString[action]
                    , phost->players[object]->name);
    }
    else {
        sprintf(str, "[Log] Player %s %s player %s doing %s\n"
                    , phost->players[p]->name
                    , gCounterString[rep.reply]
                    , phost->players[actor]->name
                    , gActionString[action]);
    }
    broadcast_log(phost, str);
    return rep.reply;
}

char ask_player_accept_challenge(Host *phost, char p, char challenger)
{
    char str[80];
    struct MsgAcceptChallenge msg;
    struct MsgReply rep;

    bzero(&msg, 16);
    bzero(&rep, 16);
    msg.opcode = enAcceptChallenge;
    msg.challenger = challenger;

    write(phost->fds[p], (char *)&msg, sizeof(struct MsgAcceptChallenge));
    read(phost->fds[p], (char *)&rep, sizeof(struct MsgReply));

    if (rep.reply > 1) {
        rep.reply = 1;
    }

    sprintf(str, "[Log] Player %s %s player %s challenge\n"
                , phost->players[p]->name
                , (rep.reply == 0) ? "Refuse" : "Accept"
                , phost->players[challenger]->name);
    broadcast_log(phost, str);
    return rep.reply;    
}

void notify_winner(Host *phost)
{
    struct MsgWin msg;
    struct MsgReply rep;

    bzero(&msg, 16);
    bzero(&rep, 16);
    msg.opcode = enWin;

    for (char i = 0; i < phost->num_players; i++) {
        if (phost->players[i]->num_influences > 0) {
            printf("[Log] Winner is %s\n", phost->players[i]->name);
            msg.winner = i;
            break;
        }
    }

    for (char i = 0; i < phost->num_players; i++) {
        write(phost->fds[i], (char *)&msg, sizeof(struct MsgWin));
    }
}

char ask_player_challenge(Host *phost, char object, char p)
{
    char str[80];
    struct MsgChallenge msg;
    struct MsgReply rep;

    bzero(&msg, 16);
    bzero(&rep, 16);
    msg.opcode = enChallengePlayer;
    msg.object = object;

    write(phost->fds[p], (char *)&msg, sizeof(struct MsgChallenge));
    read(phost->fds[p], (char *)&rep, sizeof(struct MsgReply));


    if (rep.reply > enChallenge) {
        rep.reply = enPass;
    }

    sprintf(str, "[Log] Player %s %s %s\n"
            , phost->players[p]->name
            , gCounterString[rep.reply]
            , phost->players[object]->name);
    broadcast_log(phost, str);
    return rep.reply;
}

char ask_player_block_by(Host *phost, char p, char blocked_action)
{
    char str[80];
    struct MsgBlockByWhom msg;
    struct MsgReply rep;

    bzero(&msg, 16);
    bzero(&rep, 16);
    msg.opcode = enBlockByWhom;
    msg.action = blocked_action;

    char *role_lists = gRoleCounterMatrix[blocked_action];
    char answer = 1;
    char num_roles = 0;

    for (char i = 0; i < enNumRole; i++) {
        if (role_lists[i]) {
            msg.roles[num_roles] = i;
            num_roles += 1;
        }
    }
    msg.num_roles = num_roles;


    do {
        write(phost->fds[p], (char *)&msg, sizeof(struct MsgBlockByWhom));
        read(phost->fds[p], (char *)&rep, sizeof(struct MsgReply));
        answer = rep.reply;
    }
    while (!role_lists[answer]);

    sprintf(str, "[Log] Player %s block %s by %s\n"
            , phost->players[p]->name
            , gActionString[blocked_action]
            , gRoleString[answer]);
    broadcast_log(phost, str);
    return answer;
}


char ask_player_choose_role(Host *phost, char p, char *roles, char num_roles)
{
    char str[80];
    struct MsgChooseRole msg;
    struct MsgReply rep;

    bzero(&msg, 16);
    bzero(&rep, 16);
    msg.opcode = enChooseRole;
    msg.num_roles = num_roles;

    for (unsigned char i = 0; i < num_roles; i++) {
        msg.roles[i] = roles[i];
    }

    write(phost->fds[p], (char *)&msg, sizeof(struct MsgChooseRole));
    read(phost->fds[p], (char *)&rep, sizeof(struct MsgReply));

    if (rep.reply >= num_roles) {
        rep.reply = 0;
    }

    return rep.reply;
}
