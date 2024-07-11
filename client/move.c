#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <errno.h>
#include "ncurses.h"
#include <ncurses.h>
#include "globals_client.h"
#include "requetes_to_serv.h"

// Définir la structure action

void move_4j(uint8_t a) {    
    action *act = malloc(sizeof(action));
    if (act == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    act->CODEREQ = htons(5);
    act->ID = id;
    act->NUM = htons(num_msg);
    act->ACTION = a;
    
    ssize_t bytes_sent = sendto(sockc_udp, act, sizeof(action), 0,(struct sockaddr *)&src_addr, sizeof(src_addr));
    if (bytes_sent != sizeof(action)) {
        perror("sendto");
    }
    free(act); // Libérer la mémoire allouée
    
    num_msg++;
    if (num_msg == 8192) num_msg = 0;
}