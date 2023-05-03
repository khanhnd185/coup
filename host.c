#include <time.h>
#include <stdlib.h>

#include "coup.h"
#include "interface.h"

void exchange_influence(Host *phost, unsigned char answerer, Role role1, Role role2)
{
    unsigned char ret;
    Player *player = phost->players[answerer];
    Role roles[4];

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


void take_action(Host *phost, unsigned char subject, Action action, unsigned char object)
{
    unsigned char i, role1, role2;

    notify_player_take_action(phost, subject, action, object);
    switch (action) {
            case enTax:
                phost->players[subject]->coins += 3;
                break;
            case enCoup:
            case enAssassinate:
                i = ask_player_remove(phost, object);
                remove_influence(phost->players[object], i);
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
    Action action;
    Counter counter = enPass;
    unsigned char is_accept, j, k, l, m = 0, object = 0, i = 0;
    unsigned char cant_afford = TRUE;

    while (!check_endgame(phost)) {
        do {
            if (player_lose_game(players[i])) {
                break;
            }

            while (cant_afford) {
                action = ask_player_action(phost, i);
                if (phost->players[i]->coins >= gActionSpendCoins[action]) {
                    cant_afford = FALSE;
                }
                else {
                    notify_player_message(phost, i, "Not enough money!");
                }
            }
            phost->players[i]->coins = phost->players[i]->coins - gActionSpendCoins[action];
            cant_afford = TRUE;

            if (gActionObject[action]) {
                object = ask_player_object(phost, i, action);
            }

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

                if (is_player_truth(phost->players[i], action)) {  // [TODO] Player choose lose!
                    remove_influence(phost->players[j], ask_player_remove(phost, j));
                    // [TODO] Player i has to change influence
                    take_action(phost, i, action, object);
                }
                else {
                    remove_influence(phost->players[i], ask_player_remove(phost, i));
                }
                
                break;
            }

            if (counter == enBlock) {
                Role block_by_whom = ask_player_block_by(phost, j, action);

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

                if (is_player_block_truth(phost->players[j], block_by_whom)) {  // [TODO] Player choose to lose!
                    // [TODO] Player j has to change influence
                    remove_influence(phost->players[m], ask_player_remove(phost, m));
                }
                else {
                    remove_influence(phost->players[j], ask_player_remove(phost, j));
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
