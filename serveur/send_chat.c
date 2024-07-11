#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "requetes_from_serv.h"
#include "globals_serv.h"
#include "../client/globals_client.h"



void send_message(uint8_t * buffer, int clientfd){
    msg_toserv *msg = (msg_toserv *)buffer;
    msg->CODEREQ = htons(13);
    printf("message à envoyer  : %s\n", msg -> DATA);
    msg -> LEN = strlen(msg -> DATA);
    for (int i = 0; i < nb_players_4p; ++i) {
        ssize_t sent_bytes = send(*sockets[i], msg, sizeof(msg_toserv), 0);
        if (sent_bytes < 0) {
            perror("Erreur lors de l'envoi du message");
        }
    }
}