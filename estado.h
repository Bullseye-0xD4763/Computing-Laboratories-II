//
// Created by pja on 27/02/2019.
//



#ifndef PROJ_ESTADO_H
#define PROJ_ESTADO_H

#define MAXBUFFER 1024
#define SIZE 8

/**
estado.h
Definição do estado i.e. tabuleiro. Representação matricial do tabuleiro.
*/


// definição de valores possiveis no tabuleiro
typedef enum {VAZIA, VALOR_X, VALOR_O, POSSIVEL, MELHOR} VALOR;

/**
Estrutura que armazena o estado do jogo
*/
typedef struct estado {
    VALOR peca; // peça do jogador que vai jogar!
    VALOR grelha[SIZE][SIZE];
    char modo; // modo em que se está a jogar! 0-> manual, 1-> contra computador
} ESTADO;


struct Node{
    VALOR grelha[SIZE][SIZE];
    VALOR peca;
    char modo;
    struct Node* next;
}Node;

struct Node* top;

void printa(ESTADO);


#endif //PROJ_ESTADO_H