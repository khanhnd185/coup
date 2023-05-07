#ifndef INTERFACE_H
#define INTERFACE_H

#include "coup.h"

void ask_player_name(Host *phost, char i);
void notify_winner(Host *phost);
void notify_player_message(Host *phost, char i, char *msg);
void notify_player_take_action(Host *phost, char subject, char action, char object);
void broadcast_players_info(Host *phost, char i);

char ask_player_action(Host *phost, char i);
char ask_player_remove(Host *phost, char i);
char ask_player_reveal(Host *phost, char i);
char ask_player_object(Host *phost, char i, char action);
char ask_player_challenge(Host *phost, char object, char challenger);
char ask_player_block_by(Host *phost, char answerer, char blocked_action);
char ask_player_accept_challenge(Host *phost, char answerer, char challenger);
char ask_player_choose_role(Host *phost, char answerer, char *roles, char num_roles);
char ask_player_counter(Host *phost, char actor, char action, char object, char counter);


#endif /* INTERFACE_H */
