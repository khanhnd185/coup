#ifndef INTERFACE_H
#define INTERFACE_H

#include "coup.h"

void ask_player_name(Host *phost, unsigned char i);
void notify_winner(Host *phost);
void notify_player_message(Host *phost, unsigned char i, char *msg);
void notify_player_take_action(Host *phost, unsigned char subject, Action action, unsigned char object);
Action ask_player_action(Host *phost, unsigned char i);
Counter ask_player_counter(Host *phost, unsigned char actor, Action action, unsigned char object, unsigned char counter);
unsigned char ask_player_accept_challenge(Host *phost, unsigned char answerer, unsigned char challenger);
unsigned char ask_player_object(Host *phost, unsigned char i, unsigned char action);
unsigned char ask_player_remove(Host *phost, unsigned char i);
unsigned char ask_player_reveal_role(Host *phost, unsigned char i);

Role ask_player_block_by(Host *phost, unsigned char answerer, Action blocked_action);
Counter ask_player_challenge(Host *phost, unsigned char object, unsigned char challenger);
unsigned char ask_player_choose_role(Host *phost, unsigned char answerer, Role *roles, unsigned char num_roles);


#endif /* INTERFACE_H */
