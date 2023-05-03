#include "coup.h"

unsigned int gRoleActionMatrix[5][7] = {
    // Tax  Assassinate  Exchange  Steal  BlockFA  BlockSteal  BlockAss  
      {1,   0,           0,        0,     1,       0,          0},      // Duke
      {0,   1,           0,        0,     0,       0,          0},      // Assassin
      {0,   0,           1,        0,     0,       1,          0},      // Ambassador
      {0,   0,           0,        1,     0,       1,          0},      // Captain
      {0,   0,           0,        0,     0,       0,          1}       // Contessa
};



int check_endgame(Host *phost)
{
    Player* p;
    unsigned int live_flag = 0;

    for (int i = 0; i < 2; i++) {
        p = &(phost->players[i]);
        if (p->influences > 0) {
            live_flag += 1;
        }
    }

    if (live_flag == 1) {
        return TRUE;
    }

    if (live_flag > 1) {
        return FALSE;
    }

    return ERROR;
}

void init_player(Player *pplayer)
{
    return;
}

void init_host(Host *phost, unsigned int num_player, unsigned char *player_names)
{
    return;
}



