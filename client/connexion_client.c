#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/socket.h>
#include "globals_client.h"
#include "requetes_to_serv.h"
#include "../serveur/globals_serv.h"


int connect_to_serv() {
    struct sockaddr_in6 servaddr;

    // Création du socket TCP
    if ((sockc_tcp = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
        perror("Erreur lors de la création du socket TCP");
        return -1;
    }
    int ok = 1;
    if(setsockopt(sockc_tcp, SOL_SOCKET, SO_REUSEADDR, &ok, sizeof(ok)) < 0) {
        perror("echec de SO_REUSEADDR sur TCP");
        close(sockc_tcp);
        return -1;
    }

    // Création du socket UDP
    if ((sockc_udp = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
        perror("Erreur lors de la création du socket UDP");
        close(sockc_tcp);
        return -1;
    }
    ok = 1;
    if(setsockopt(sockc_udp, SOL_SOCKET, SO_REUSEADDR, &ok, sizeof(ok)) < 0) {
        perror("echec de SO_REUSEADDR");
        close(sockc_tcp);
        close(sockc_udp);
        return -1;
    }
    // Configuration de l'adresse du serveur
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_port = htons(PORT_TCP);
    servaddr.sin6_addr = in6addr_any;

    // Connexion au serveur TCP
    if (connect(sockc_tcp, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Erreur lors de la connexion au serveur TCP");
        close(sockc_tcp);
        close(sockc_udp);
        return -1;
    }
    
    puts("Connexion au serveur réussie.");
    choose_mode();
    return 0;
}
