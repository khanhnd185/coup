#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "coup.h"
#include "interface.h"

static char check_endgame(Host *phost)
{
    char num_alive_players = 0;

    for (char i = 0; i < phost->num_players; i++) {
        if (phost->players[i]->num_influences > 0) {
            num_alive_players += 1;
        }
    }

    if (num_alive_players > 1) {
        return FALSE;
    }

    return TRUE;
}

static char player_lose_game(Player *player) {
    return (char)(player->num_influences == 0);
}

static void remove_influence(Host *phost, char p, char i)
{
    Player* pplayer = phost->players[p];

    do {
        if (pplayer->num_influences < 1) {
            break;
        }

        if (pplayer->num_influences == 1) {
            pplayer->num_influences = 0;
            break;
        }

        if (i > 1) {
            i == 1;
        }

        pplayer->num_influences = 1;
        if (i == 1) {
            break;
        }

        pplayer->influences[0] = pplayer->influences[1];
    }
    while (0);

    broadcast_players_info(phost, p);
}


static void change_influence(Host *phost, char p, char i)
{
    srand(time(NULL));
    phost->players[p]->influences[i] = rand() % enNumRole;
    broadcast_players_info(phost, p);
}

static void exchange_influence(Host *phost, char p, char role1, char role2)
{
    char ret;
    Player *player = phost->players[p];
    char roles[4];

    roles[0] = role1;
    roles[1] = role2;
    roles[2] = player->influences[0];

    if (player->num_influences == 1) {
        ret = ask_player_choose_role(phost, p, roles, 3);
        player->influences[0] = roles[ret];
        broadcast_players_info(phost, p);
        return;
    }

    roles[3] = player->influences[1];
    ret = ask_player_choose_role(phost, p, roles, 4);
    player->influences[0] = roles[ret];
    roles[ret] = roles[3];
    ret = ask_player_choose_role(phost, p, roles, 3);
    player->influences[1] = roles[ret];
    broadcast_players_info(phost, p);
}


void take_action(Host *phost, char subject, char action, char object)
{
    char role1, role2;

    notify_player_take_action(phost, subject, action, object);
    switch (action) {
            case enTax:
                phost->players[subject]->coins += 3;
                broadcast_players_info(phost, subject);
                break;
            case enCoup:
            case enAssassinate:
                remove_influence(phost, object, ask_player_remove(phost, object));
                broadcast_players_info(phost, object);
                break;
            case enExchange:
                srand(time(NULL));
                role1 = rand() % enNumRole;
                role2 = rand() % enNumRole;
                exchange_influence(phost, subject, role1, role2);
                broadcast_players_info(phost, subject);
                break;
            case enSteal:
                phost->players[subject]->coins += 2;
                phost->players[object]->coins -= 2;
                broadcast_players_info(phost, subject);
                broadcast_players_info(phost, object);
                break;
            case enForeignAid:
                phost->players[subject]->coins += 2;
                broadcast_players_info(phost, subject);
                break;
            case enIncome:
            default:
                phost->players[subject]->coins += 1;
                broadcast_players_info(phost, subject);
                break;
    }
}


void run(Host *phost)
{
    Player **players = phost->players;
    char action;
    char counter = enPass;
    char role_index, is_accept, j, k, l, m = 0, object = 0, i = 0;
    char cant_afford = TRUE;
    char object_broke = FALSE;

    while (!check_endgame(phost)) {
        do {
            if (player_lose_game(players[i])) {
                break;
            }

            do {
                object_broke = FALSE;

                while (cant_afford) {
                    action = ask_player_action(phost, i);
                    if (phost->players[i]->coins >= gActionSpendCoins[action]) {
                        cant_afford = FALSE;
                    }
                    else {
                        notify_player_message(phost, i, "has not enough money!");
                    }
                }
                phost->players[i]->coins = phost->players[i]->coins - gActionSpendCoins[action];
                
                if (gActionSpendCoins[action]) {
                    broadcast_players_info(phost, i);
                }
                cant_afford = TRUE;

                if (gActionObject[action]) {
                    object = ask_player_object(phost, i, action);

                    if ((action == enSteal) &&
                        (phost->players[object]->coins < 2)) {
                        object_broke = TRUE;
                        notify_player_message(phost, object, "has not enough coins to be stealed!");
                    }
                }
                
            }
            while (object_broke); // [TODO] CHeck coin

            if (gChallengeAction[action] == FALSE) {
                take_action(phost, i, action, object);
                break;
            }

            for (k  = (i+1); k < (phost->num_players + i); k++) {
                j = (k % phost->num_players);
                if (player_lose_game(players[j])) {
                    continue;
                }
                counter = ask_player_counter(phost, i, action, object, j);

                if (counter != enPass) {
                    break;
                }
            }

            if (k == phost->num_players + i) { // No one counter
                take_action(phost, i, action, object);
                break;
            }

            if (counter == enChallenge) {
                role_index = ask_player_reveal(phost, i);
                if (gActionRoleMatrix[action][phost->players[i]->influences[role_index]]) {
                    remove_influence(phost, j, ask_player_remove(phost, j));
                    change_influence(phost, i, role_index);
                    take_action(phost, i, action, object);
                }
                else {
                    remove_influence(phost, i, role_index);
                }
                
                break;
            }

            if (counter == enBlock) {
                char block_by_whom = ask_player_block_by(phost, j, action);

                for (l  = (j+1); l < (phost->num_players + j); l++) {
                    m = (l % phost->num_players);
                    if (player_lose_game(players[m])) {
                        continue;
                    }
                    if (ask_player_challenge(phost, j, m) == enChallenge) {
                        break;
                    }
                }

                if (l == phost->num_players + j) { // No one challenge                    
                    break;
                }

                role_index = ask_player_reveal(phost, j);
                if (block_by_whom == phost->players[j]->influences[role_index]) {
                    change_influence(phost, j, role_index);
                    remove_influence(phost, m, ask_player_remove(phost, m));
                }
                else {
                    remove_influence(phost, j, role_index);
                    take_action(phost, i, action, object);
                }
            }
        }
        while (0);

        i++;
        i = i % phost->num_players;
    }

    notify_winner(phost);
}
