#include <stdio.h>
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

void print_player_public_info(Player *player)
{
    printf("[%s] ", player->name);
    for (char i = 0; i < player->num_influences; i++) {
        printf("[]");
    }
    printf("[%d]", player->coins);
}

void print_player_private_info(Player *player)
{
    printf("[%s] ", player->name);
    for (char i = 0; i < player->num_influences; i++) {
        printf("[%s]", gRoleString[player->influences[i]]);
    }
    printf("[%d]", player->coins);
}

