#ifndef HANDLER_H
#define HANDLER_H

#include "coup.h"

char handle_message(int sockfd, char *m, Player **players, char num_players, char p);

#endif /* HANDLER_H */
