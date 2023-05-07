#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

#include "msg.h"
#include "handler.h"

void func(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

int connect_server(void)
{
    int n, sockfd;
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
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
        != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    return sockfd;
}

int main(int argc, char **argv)
{
    int n = 0;
    int sockfd, c;
    char num_players, my_position;
    struct MsgName msg;
    struct MsgReply rep;

    bzero(&rep, 16);
    bzero(&msg, 16);

    while ((c = getopt (argc, argv, "n:")) != -1) {
        switch (c) {
            case 'n':
                strcpy(rep.name, optarg);
                break;
            default:
                abort ();
        }
    }

    sockfd = connect_server();

    read(sockfd, (char *)&msg, sizeof(struct MsgName));
    num_players = msg.name[0];
    my_position = msg.number;


    Player **players = malloc(sizeof(Player *) * num_players);
    for (char i = 0; i < num_players; i++) {
        players[i] = malloc(sizeof(Player));
        bzero(players[i], 16);
    }

    players[my_position]->coins = msg.coins;
    players[my_position]->influences[0] = msg.role[0];
    players[my_position]->influences[1] = msg.role[1];
    players[my_position]->num_influences = msg.numinf;
    strcpy(players[my_position]->name, rep.name);

    printf("Your influences are %s and %s.You are player %d(th). You have %d coins\n"
            , gRoleString[players[my_position]->influences[0]]
            , gRoleString[players[my_position]->influences[1]]
            , my_position
            , players[my_position]->coins);
    write(sockfd, (char *)&rep, sizeof(struct MsgReply));


    printf("There are %d players: ", num_players);
    for (char i = 0; i < (num_players - 1); i++) {
        bzero(&msg, 16);
        read(sockfd, (char *)&msg, sizeof(struct MsgName));
        strcpy(players[msg.number]->name, msg.name);
        printf("[%d:%s] ", msg.number, players[msg.number]->name);
    }
    printf("\n");

  
    while (msg.opcode != enWin) {
        read(sockfd, (char *)&msg, 16);
        bzero(&rep, 16);

        rep.reply = handle_message(sockfd, (char *) &msg, players, num_players, my_position);
        if ((msg.opcode != enWin) && (msg.opcode != enName) && (msg.opcode != enMsgLog)) {
            write(sockfd, (char *)&rep, sizeof(struct MsgReply));
        }
    }

//    func(sockfd);
    close(sockfd);
}
