#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include "globals_client.h"
#include "requetes_to_serv.h"

#define BUF_SIZE 1024

int stop_thread = 0;

void* ready_thread(void* args) {
    int *to_flag = (int *) args;
    int s = 0;
    while(!stop_thread || s < 10){
        sleep(1);
        s++;
    }
    if(s==10) *to_flag = 1;
    pthread_exit(NULL);
}

void announce_ready_4j(int id, int eq) {
    pthread_t thread;
    int *timeout_flag = malloc(sizeof(int));
    if (pthread_create(&thread, NULL, ready_thread, &timeout_flag) != 0) {
        perror("Erreur lors de la création du thread");
        free(timeout_flag);
        return;
    }

    puts("Veuillez indiquer quand vous êtes prêt (envoyez un message quelconque)\n");
    char buffer[BUF_SIZE];
    while (*timeout_flag == 0) {
        if (fgets(buffer, BUF_SIZE, stdin) != NULL) {
            ready *r = malloc(sizeof(ready));
            r->CODEREQ = htons(3);
            r->ID = id;
            r->EQ = eq;
            if (send(sockc_tcp, r, sizeof(ready), 0) <= 0) {
                perror("Erreur lors de l'envoi du message de prêt");
            }
            stop_thread = 1;
            free(r);
            break;
        }
    }
    if (*timeout_flag == 1) {
        printf("Vous n'avez pas indiqué que vous étiez prêt dans le délai imparti.\n");
        ready* r = malloc(sizeof(ready));
        r->CODEREQ = htons(21);
        r->ID = id;
        r->EQ = eq;
        if (send(sockc_tcp, r, sizeof(ready), 0) <= 0) {
            perror("Erreur lors de l'envoi du message d'expiration du délai");
        }
        free(r);
    }
}

void announce_ready_2v2(int id, int eq) {
    pthread_t thread;
    int *timeout_flag = malloc(sizeof(int));
    if (pthread_create(&thread, NULL, ready_thread, &timeout_flag) != 0) {
        perror("Erreur lors de la création du thread");
        free(timeout_flag);
        return;
    }

    puts("Veuillez indiquer quand vous êtes prêt (envoyez un message quelconque)\n");
    char buffer[BUF_SIZE];
    while (*timeout_flag == 0) {
        if (fgets(buffer, BUF_SIZE, stdin) != NULL) {
            ready *r = malloc(sizeof(ready));
            r->CODEREQ = htons(4);
            r->ID = id;
            r->EQ = eq;
            if (send(sockc_tcp, r, sizeof(ready), 0) <= 0) {
                perror("Erreur lors de l'envoi du message de prêt");
            }
            stop_thread = 1;
            free(r);
            break;
        }
    }
    if (*timeout_flag == 1) {
        printf("Vous n'avez pas indiqué que vous étiez prêt dans le délai imparti.\n");
        ready* r = malloc(sizeof(ready));
        r->CODEREQ = htons(21);
        r->ID = id;
        r->EQ = eq;
        if (send(sockc_tcp, r, sizeof(ready), 0) <= 0) {
            perror("Erreur lors de l'envoi du message d'expiration du délai");
        }
        free(r);
    }
}