#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <arpa/inet.h>
#include <net/if.h>
#include "globals_client.h"
#include "../serveur/globals_serv.h"
#include "requetes_to_serv.h"

int num_msg;

int *abo_multidiff(uint8_t *buffer){
    gameSetup *gs = (gameSetup*) buffer;

    struct sockaddr_in6 adr;
    memset(&adr, 0, sizeof(adr));
    adr.sin6_family = AF_INET6;
    adr.sin6_port = gs->PORTMDIFF;
    adr.sin6_addr = in6addr_any;
    int yes = 1;
    if(setsockopt(sockc_udp,  SOL_SOCKET, SO_REUSE, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        end();
    }

    if(bind(sockc_udp, (struct sockaddr*) &adr, sizeof(adr)) < 0) {
        perror("bind");
        end();
    }

    int ifindex = if_nametoindex(MULTICAST);
    if (ifindex == 0) {
        perror("if_nametoindex");
        end();
    }

    struct ipv6_mreq group;
    memcpy(&group.ipv6mr_multiaddr, gs->ADRMDIFF, sizeof(group.ipv6mr_multiaddr));
    group.ipv6mr_interface = ifindex;

    if (setsockopt(sockc_udp, IPPROTO_IPV6, IPV6_JOIN_GROUP, &group, sizeof(group)) < 0) {
        perror("echec de abonnement groupe");
        close(sockc_udp);
        close(sockc_tcp);
        end();
    }

    char ip[INET6_ADDRSTRLEN];
    if (inet_ntop(AF_INET6, &group.ipv6mr_multiaddr, ip, sizeof(ip))) {
        printf("Je me suis abonné à l'adresse %s sur le port %d.\n", ip, ntohs(gs->PORTMDIFF));
    } else {
        perror("Erreur lors de la récupération de l'adresse du socket");
        end();
    }
    num_msg = 1;
    if(gs -> EQ == 255) announce_ready_4j(gs -> ID, gs -> EQ);
    else announce_ready_2v2(gs -> ID, gs -> EQ);
    int *info = malloc(2 * sizeof(int));
    info[0] = gs -> ID;
    info[1] = gs -> EQ;
    return info;
}