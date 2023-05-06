#ifndef MSG_H
#define MSG_H

typedef enum enMessageType {
    enName,
    enWin,
    enMessage,
    enChooseAction,
    enChooseCounter,
    enAcceptChallenge,
    enChooseObject,
    enRemoveInfluence,
    enRevealInfluence,
    enBlockByWhom,
    enChallengePlayer,
    enChooseRole,
    enNumMessage
} MessageType;

struct MsgReply {
    char opcode;
    char reply;
    char pad[4];
    char name[10];
};

struct MsgName {
    char opcode;
    char number;
    char numinf;
    char coins;
    char role[2];
    char name[10];
};

struct MsgWin {
    char opcode;
    char winner;
    char pad[14];
};

struct MsgMessage {
    char opcode;
    char msg[15];
};

struct MsgChooseAction {
    char opcode;
    char pad[15];
};

struct MsgChooseCounter {
    char opcode;
    char subject;
    char action;
    char object;
    char pad[12];
};

struct MsgAcceptChallenge {
    char opcode;
    char challenger;
    char pad[14];
};

struct MsgChooseObject {
    char opcode;
    char action;
    char num_objects;
    char players[13];
};

struct MsgRemoveInfluence {
    char opcode;
    char num_influences;
    char influences[2];
    char pad[12];
};

struct MsgRevealInfluence {
    char opcode;
    char num_influences;
    char influences[2];
    char pad[12];
};

struct MsgBlockByWhom {
    char opcode;
    char action;
    char num_roles;
    char roles[2];
    char pad[11];
};

struct MsgChallenge {
    char opcode;
    char object;
    char pad[14];
};

struct MsgChooseRole {
    char opcode;
    char roles[4];
    char num_roles;
    char pad[10];
};

#endif /* MSG_H */