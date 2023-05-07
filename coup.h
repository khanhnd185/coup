#ifndef COUP_H
#define COUP_H

#define TRUE 1
#define FALSE 0
#define ERROR -1

typedef enum enRole{
    enDuke,
    enAssassin,
    enAmbassador,
    enCaptain,
    enContessa,
    enNumRole
} Role;

typedef enum enAction {
    enTax,
    enAssassinate,
    enExchange,
    enSteal,
    enIncome,
    enForeignAid,
    enCoup,
    enNumAction,
} Action;

typedef enum enCounter {
    enPass,
    enChallenge,
    enBlock,
    enNumCounter
} Counter;

typedef struct {
    char num_influences;
    char coins;
    Role influences[2];
    char name[10];
} Player;

typedef struct {
    char from;
    char *to;
} Int2Str;

typedef struct {
    Player **players;
    int *fds;
    char num_players;
} Host;

extern unsigned char gBlockAction[enNumAction];
extern unsigned char gActionObject[enNumAction];
extern unsigned char gChallengeAction[enNumAction];
extern unsigned char gActionSpendCoins[enNumAction];
extern unsigned char gActionRoleMatrix[enNumAction][enNumRole];
extern unsigned char gRoleCounterMatrix[enNumAction][enNumRole];

extern char *gRoleString[enNumRole];
extern char *gActionString[enNumAction];
extern char *gCounterString[enNumCounter];

void print_player_public_info(Player *player);
void print_player_private_info(Player *player);

#endif /* COUP_H */