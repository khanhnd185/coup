#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "coup.h"
#include "interface.h"

void change_influence(Host *phost, char p, char index)
{
    char str[32];
    sprintf(str, "change %s", gRoleString[phost->players[p]->influences[index]]);
    notify_player_message(phost, p, str);
    
    srand(time(NULL));
    phost->players[p]->influences[index] = rand() % enNumRole;
}

void exchange_influence(Host *phost, char answerer, char role1, char role2)
{
    char ret;
    Player *player = phost->players[answerer];
    char roles[4];

    roles[0] = role1;
    roles[1] = role2;
    roles[2] = player->influences[0];

    if (player->num_influences == 1) {
        ret = ask_player_choose_role(phost, answerer, roles, 3);
        player->influences[0] = roles[ret];
        return;
    }

    roles[3] = player->influences[1];
    ret = ask_player_choose_role(phost, answerer, roles, 4);
    player->influences[0] = roles[ret];
    roles[ret] = roles[3];
    ret = ask_player_choose_role(phost, answerer, roles, 3);
    player->influences[1] = roles[ret];
}


void take_action(Host *phost, char subject, char action, char object)
{
    char role1, role2;

    notify_player_take_action(phost, subject, action, object);
    switch (action) {
            case enTax:
                phost->players[subject]->coins += 3;
                break;
            case enCoup:
            case enAssassinate:
                remove_influence(phost, object, ask_player_remove(phost, object));
                break;
            case enExchange:
                srand(time(NULL));
                role1 = rand() % enNumRole;
                role2 = rand() % enNumRole;
                exchange_influence(phost, subject, role1, role2);
                break;
            case enSteal:
                phost->players[subject]->coins += 2;
                phost->players[object]->coins -= 2;
                break;
            case enForeignAid:
                phost->players[subject]->coins += 2;
                break;
            case enIncome:
            default:
                phost->players[subject]->coins += 1;
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

            if ((gBlockAction[action] == FALSE) && (gChallengeAction[action] == FALSE)) {
                take_action(phost, i, action, object);
                break;
            }

            for (k  = (i+1); k < (phost->num_players + i); k++) {
                j = (k % phost->num_players);
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
                is_accept = ask_player_accept_challenge(phost, i, j);

                if (is_accept == FALSE) {
                    break;
                }

                role_index = ask_player_reveal(phost, i);
                if (gActionRoleMatrix[action][phost->players[i]->influences[role_index]]) {
                    remove_influence(phost, j, ask_player_remove(phost, j));
                    change_influence(phost, i, role_index);
                    take_action(phost, i, action, object);
                }
                else {
                    remove_influence(phost, i, ask_player_remove(phost, i));
                }
                
                break;
            }

            if (counter == enBlock) {
                char block_by_whom = ask_player_block_by(phost, j, action);

                for (l  = (j+1); l < (phost->num_players + j); l++) {
                    m = (l % phost->num_players);
                    if (ask_player_challenge(phost, j, m) == enChallenge) {
                        break;
                    }
                }

                if (l == phost->num_players + j) { // No one challenge                    
                    break;
                }

                is_accept = ask_player_accept_challenge(phost, j, m);

                if (is_accept == FALSE) {
                    take_action(phost, i, action, object);
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
