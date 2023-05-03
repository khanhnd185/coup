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
    enStealm,
    enBlockForeignAid,
    enBlockStealing,
    enBlockAssassination,
    enIncome,
    enForeignAid,
    enCoup,
    enNumAction
} Action;

typedef struct {
    unsigned int influences;
    unsigned char name[20];
    Role role[2];
} Player;


typedef struct {
    Action action;
    Player *whom;
} FullAction;

typedef struct {
    Player *players;
    unsigned int num_players;
} Host;

extern unsigned int gRoleActionMatrix[5][7];

int check_endgame(Host *phost);
void init_player(Player *pplayer);
void init_host(Host *phost, unsigned int num_player, unsigned char *player_names);

#endif /* COUP_H */