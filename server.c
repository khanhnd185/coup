#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#define MAX 16
#define PORT 8080
#define SA struct sockaddr

#include "host.h"
#include "interface.h"

extern void introduce_players_name(Host *phost, char p);

void func(int connfd)
{
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, MAX);

        read(connfd, buff, sizeof(buff));
        printf("From client: %s\t To client : ", buff);
        bzero(buff, MAX);
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;

        write(connfd, buff, sizeof(buff));

        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

int* connect_players(int num_players)
{
    int *connfds;
    int sockfd, len;
    char buff[MAX];
    unsigned int i = 0;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    connfds = malloc(sizeof(int) * (num_players + 1));

    while (i < num_players) {
        connfds[i] = accept(sockfd, (SA*)&cli, &len);
        if (connfds[i] < 0) {
            printf("server accept failed...\n");
        }
        else
            printf("server accept the player %d.\n", i);
            i++;
    }

    connfds[num_players] = sockfd;

    return connfds;
}

void free_all(Host* phost)
{
    for (char i = 0; i < phost->num_players; i++) {
        free(phost->players[i]);
    }
    free(phost->players);
    free(phost->fds);
    free(phost);
}

void main(int argc, char **argv)
{
    char n = 0;
    char buff[MAX];
    char i, c, num_players;

    while ((c = getopt (argc, argv, "n:")) != -1) {
        switch (c) {
            case 'n':
                num_players = atoi(optarg);
                break;
            default:
                abort ();
        }
    }

    Player **players = malloc(sizeof(Player *) * num_players);
    Host *phost = malloc(sizeof(Host));

    phost->num_players = num_players;
    phost->players = players;
    phost->fds = connect_players(num_players);

    for (char i = 0; i < num_players; i++) {
        players[i] = malloc(sizeof(Player));
        players[i]->coins = 2;
        players[i]->num_influences = 2;
        players[i]->influences[0] = rand() % enNumRole;
        players[i]->influences[1] = rand() % enNumRole;
        ask_player_name(phost, i);

        printf("Player %s [%s][%s]\n\n"
                , players[i]->name
                , gRoleString[players[i]->influences[0]]
                , gRoleString[players[i]->influences[1]]);
    }

    for (char i = 0; i < num_players; i ++) {
        introduce_players_name(phost, i);
    }

    run(phost);
    notify_winner(phost);

    close(phost->fds[num_players]);
    free_all(phost);
}
