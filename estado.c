//
// Created by pja on 28/02/2019.
//
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "estado.h"

// exemplo de uma função para imprimir o estado (Tabuleiro)
void printa(ESTADO e)
{
    char c = ' ';

    printf("\n  1 2 3 4 5 6 7 8 \n");

    for (int i = 0; i < SIZE; i++) {
        printf("%d ",i+1);
        for (int j = 0; j < SIZE; j++) {
            switch (e.grelha[i][j]) {
                case VALOR_O: {
                    c = 'O';
                    break;
                }
                case VALOR_X: {
                    c = 'X';
                    break;
                }
                case VAZIA: {
                    c = '-';
                    break;
                }
                case POSSIVEL: {
                    c = '.';
                    break;
                }
                case MELHOR: {
                    c = '?';
                    break;
                }
            }
            printf("%c ", c);

        }
        printf("\n");
    }

    printf("\n");

}

