#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ncurses.h>
#include "globals_client.h"
#include "requetes_to_serv.h"

#define BUF_SIZE 256

typeGame *create_player(int codereq){
    typeGame *player = malloc(sizeof(typeGame));
    player -> CODEREQ = htons(codereq);
    player -> EQ = htons(-1);
    player -> ID = htons(-1);
    return player;
}

void choose_mode(){
    puts("Veuillez indiquer si vous voulez intégrer :\n<1> Une partie à 4 joueurs\n<2> Une partie à 2v2\n<p> Quitter le jeu\n");
    typeGame *t = malloc(sizeof(typeGame));
    while(1){
        char buffer[BUF_SIZE];  // Tampon pour lire les entrées
        if (fgets(buffer, BUF_SIZE, stdin) != NULL) {  // Lire l'entrée du terminal
            char premier_char = buffer[0];  // Premier caractère de l'entrée
            switch (premier_char) {
                case '1':
                    t = create_player(1);
                    if(send(sockc_tcp, t, sizeof(typeGame), 0)<=0){
                        perror("send");
                        free(t);
                        end();
                    }
                    return;
                case '2':
                    t = create_player(2);
                    if(send(sockc_tcp, t, sizeof(typeGame), 0)<=0){
                        perror("send");
                        free(t);
                        end();
                    }
                    return;
                case 'p':
                    printf("Au revoir.\n");
                    free(t);
                    end();
                case 'P':
                    printf("Au revoir.\n");
                    free(t);
                    end();
                default:
                    printf("Vous ne pouvez écrire que <1>, <2> ou <p>\n");
                    break;
            }
        }
    }
}
