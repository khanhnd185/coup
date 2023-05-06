#include "coup.h"

unsigned char gActionRoleMatrix[enNumAction][enNumRole] = {
    // Duke  Assa  Amba  Capt  Cont
      {1,    0,    0,    0,    0}, // Tax
      {0,    1,    0,    0,    0}, // Assassinate
      {0,    0,    1,    0,    0}, // Exchange
      {0,    0,    0,    1,    0}, // Steal
      {1,    1,    1,    1,    1}, // Income
      {1,    1,    1,    1,    1}, // Foreign Aid
      {1,    1,    1,    1,    1}, // Coup
};

unsigned char gRoleCounterMatrix[enNumAction][enNumRole] = {
    // Duke  Assa  Amba  Capt  Cont
      {0,    0,    0,    0,    0}, // Tax
      {0,    0,    0,    0,    1}, // Assassinate
      {0,    0,    0,    0,    0}, // Exchange
      {0,    0,    1,    1,    0}, // Steal
      {0,    0,    0,    0,    0}, // Income
      {1,    0,    0,    0,    0}, // Foreign Aid
      {0,    0,    0,    0,    0}, // Coup
};

unsigned char gChallengeAction[enNumAction] = {
      1, // Tax
      1, // Assassinate
      1, // Exchange
      1, // Steal
      0, // Income
      1, // Foreign Aid
      0, // Coup
};

unsigned char gBlockAction[enNumAction] = {
      0, // Tax
      1, // Assassinate
      0, // Exchange
      1, // Steal
      0, // Income
      1, // Foreign Aid
      0, // Coup
};

unsigned char gActionObject[enNumAction] = {
      0, // Tax
      1, // Assassinate
      0, // Exchange
      1, // Steal
      0, // Income
      0, // Foreign Aid
      1, // Coup
};

unsigned char gActionSpendCoins[enNumAction] = {
      0, // Tax
      3, // Assassinate
      0, // Exchange
      0, // Steal
      0, // Income
      0, // Foreign Aid
      7, // Coup
};

char *gActionString[enNumAction] = {
      "Tax",
      "Assassinate",
      "Exchange",
      "Steal",
      "Income",
      "Foreign Aid",
      "Coup",
};

char *gRoleString[enNumRole] = {
    "Duke",
    "Assassin",
    "Ambassador",
    "Captain",
    "Contessa",
};

char *gCounterString[enNumCounter] = {
    "Ignore",
    "Challenge",
    "Block",
};

char check_endgame(Host *phost)
{
    char live_flag = 0;

    for (char i = 0; i < 2; i++) {
        if (phost->players[i]->num_influences > 0) {
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

void remove_influence(Host *phost, char p, char i) {
    Player* pplayer = phost->players[p];

    if (pplayer->num_influences < 1) {
        return;
    }

    if (pplayer->num_influences == 1) {
        pplayer->num_influences = 0;
        return;
    }

    if (i > 1) {
        i == 1;
    }

    pplayer->num_influences = 1;
    if (i == 1) {
        return;
    }

    pplayer->influences[0] = pplayer->influences[1];
}

char player_lose_game(Player *player) {
    return (char)(player->influences == 0);
}
