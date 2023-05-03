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
    unsigned char num_influences;
    unsigned char name[16];
    Role influences[2];
    unsigned char coins;
} Player;

typedef struct {
    unsigned char from;
    unsigned char *to;
} Int2Str;

typedef struct {
    Action action;
    Player *whom;
} FullAction;

typedef struct {
    Player **players;
    unsigned int num_players;
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


char check_endgame(Host *phost);
void init_player(Player *pplayer);
void init_host(Host *phost, unsigned char num_player, unsigned char *player_names);

void remove_influence(Player* pplayer, unsigned int i);
unsigned char player_lose_game(Player *player);
unsigned char is_player_truth(Player* player, Action action);
unsigned char is_player_block_truth(Player* player, Role blocking_role);

#endif /* COUP_H */