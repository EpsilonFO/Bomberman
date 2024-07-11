#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "globals_serv.h"
#include "../client/globals_client.h"
#include "requetes_from_serv.h"

int sock_tcp, sock_udp;
struct sockaddr_in6 servaddr_tcp, servaddr_udp, multicastAddr_4j, multicastAddr_2v2;
int nb_players_4p, nb_players_2v2, nb_games_4p, nb_games_2v2, ready_4p, ready_2v2 = 0;
int **sockets;

void *client_thread(void *args) {
    player *p = (player *)args;
    int clientfd = p->clientfd;
    uint8_t *cases = p->cases;
    game(clientfd, cases);
    free(p);
    free(cases);  // Libérer la mémoire allouée pour 'cases'
    return NULL;
}

int main() {
    // Allocation du tableau de sockets
    sockets = malloc(sizeof(int*) * 4);
    if (sockets == NULL) {
        perror("Erreur lors de l'allocation du tableau de sockets");
        return -1;
    }

    // Création du socket TCP
    if ((sock_tcp = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
        perror("Erreur lors de la création du socket TCP");
        return -1;
    }
    int yes = 1;
    if (setsockopt(sock_tcp, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        perror("setsockopt (SO_REUSEADDR) failed");
        return -1;
    }

    // Création du socket UDP pour écouter les clients
    if ((sock_udp = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
        perror("Erreur lors de la création du socket UDP");
        return -1;
    }
    yes = 1;
    if (setsockopt(sock_udp, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        perror("setsockopt (SO_REUSEADDR) failed");
        return -1;
    }

    // Configuration de l'adresse du serveur TCP
    memset(&servaddr_tcp, 0, sizeof(servaddr_tcp));
    servaddr_tcp.sin6_family = AF_INET6;
    servaddr_tcp.sin6_port = htons(PORT_TCP);
    servaddr_tcp.sin6_addr = in6addr_any;

    // Configuration de l'adresse du serveur UDP
    memset(&servaddr_udp, 0, sizeof(servaddr_udp));
    servaddr_udp.sin6_family = AF_INET6;
    servaddr_udp.sin6_port = htons(PORT_UDP);
    servaddr_udp.sin6_addr = in6addr_any;

    // Configuration de l'adresse multicast 4 joueurs
    memset(&multicastAddr_4j, 0, sizeof(multicastAddr_4j));
    multicastAddr_4j.sin6_family = AF_INET6;
    multicastAddr_4j.sin6_port = htons(PORT_MDIFF_4J);
    if (inet_pton(AF_INET6, ADDR_MDIFF_4J, &multicastAddr_4j.sin6_addr) <= 0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }
    // Configuration de l'adresse multicast 2v2
    memset(&multicastAddr_2v2, 0, sizeof(multicastAddr_2v2));
    multicastAddr_2v2.sin6_family = AF_INET6;
    multicastAddr_2v2.sin6_port = htons(PORT_MDIFF_2V2);
    if (inet_pton(AF_INET6, ADDR_MDIFF_2V2, &multicastAddr_2v2.sin6_addr) <= 0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

    // Association des sockets à leurs adresses et ports respectifs
    if (bind(sock_tcp, (const struct sockaddr *)&servaddr_tcp, sizeof(servaddr_tcp)) < 0) {
        perror("Erreur lors de la liaison du socket TCP");
        close(sock_tcp);
        close(sock_udp);
        return -1;
    }

    if (bind(sock_udp, (const struct sockaddr *)&servaddr_udp, sizeof(servaddr_udp)) < 0) {
        perror("Erreur lors de la liaison du socket UDP");
        close(sock_tcp);
        close(sock_udp);
        return -1;
    }

    // Écoute des connexions TCP
    if (listen(sock_tcp, 5) < 0) {
        perror("Erreur lors de l'écoute des connexions TCP");
        close(sock_tcp);
        close(sock_udp);
        return -1;
    }
    
    uint8_t* cases = malloc(HEIGHT * WIDTH * sizeof(uint8_t));  // Utiliser uint8_t*
    if (cases == NULL) {
        perror("Erreur lors de l'allocation de mémoire pour 'cases'");
        close(sock_tcp);
        close(sock_udp);
        return -1;
    }

    // Boucle pour accepter les connexions et créer des threads
    int i = 0;
    while (1) {
        struct sockaddr_in6 clientaddr;
        socklen_t clientlen = sizeof(clientaddr);

        // Accepter une connexion TCP
        int *clientfd = malloc(sizeof(int)); // Allouer de la mémoire pour le descripteur du client
        if (clientfd == NULL) {
            perror("Erreur lors de l'allocation de mémoire pour le descripteur du client");
            continue;
        }
        *clientfd = accept(sock_tcp, (struct sockaddr *)&clientaddr, &clientlen);
        if (*clientfd < 0) {
            perror("Erreur lors de l'acceptation de la connexion TCP");
            free(clientfd);
            continue;
        }

        if (i < 4) {
            sockets[i] = clientfd;
            i++;
        } else {
            // Gestion de l'erreur si plus de 4 clients se connectent
            printf("Nombre maximum de clients atteint.\n");
            close(*clientfd);
            free(clientfd);
            continue;
        }

        puts("Un client vient de se connecter.");

        // Créer un nouveau thread pour gérer le client
        pthread_t thread_id;
        player *p = (player *)malloc(sizeof(player));
        if (p == NULL) {
            perror("Erreur lors de l'allocation de mémoire pour le joueur");
            close(*clientfd);
            free(clientfd);
            continue;
        }
        p->clientfd = *clientfd;
        p->cases = cases;

        if (pthread_create(&thread_id, NULL, client_thread, p) != 0) {
            perror("Erreur lors de la création du thread");
            close(p->clientfd);
            free(p);
            free(cases);
            free(clientfd);  // Libérer la mémoire allouée pour le descripteur du client
            continue;
        }

        pthread_detach(thread_id);  // Détacher le thread pour qu'il se libère automatiquement à la fin
    }

    close(sock_tcp);
    close(sock_udp);
    return 0;
}