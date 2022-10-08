#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include "AI.h"
#include "estado.h"
#include "Interpretador.h"


ESTADO temp = {0};


//-----------------------------Novo Jogo--------------------------------------------------------------------------------
ESTADO novojogoAI(ESTADO ai, char *peca){
    int i,j;
    for (i = 0; i<SIZE; i++)           /* set all array positions's value to '-'*/
        for (j = 0; j<SIZE; j++)
            ai.grelha[i][j] = VAZIA;

    ai.grelha[3][4] = VALOR_O;
    ai.grelha[4][3] = VALOR_O;       /* set initial pieces  */
    ai.grelha[3][3] = VALOR_X;
    ai.grelha[4][4] = VALOR_X;

    printf("Jogo contra AI \n");

    switch (toupper(peca[0])){
        case 'X':
            ai.peca = VALOR_X;
            break;
        case 'O':
            ai.peca = VALOR_O;
            break;
        default:
            printf("Erro : A peça tem de ser X ou O (rtfm)\n");
    }

    return ai;
}

double getScore(ESTADO ai){
    VALOR opponent, player;
    int i,j,score = 0;

    if (ai.peca == VALOR_O) {
        player = VALOR_O;
        opponent = VALOR_X;
    } else {
        player = VALOR_X;
        opponent = VALOR_O;
    }

    for (i = 0; i<SIZE;i++)
        for (j = 0; j<SIZE;j++){
            if (ai.grelha[i][j] == player)
                ++score;
            if (ai.grelha[i][j] == opponent)
                --score;
        }

    return score;
}

double heuristic(ESTADO ai){
    VALOR player, opponent;
    int i,j;
    double hvalue = 0;

    if (ai.peca == VALOR_X){
        player = VALOR_X;
        opponent = VALOR_O;
    }else{
        player = VALOR_O;
        opponent = VALOR_X;
    }


    double board[SIZE][SIZE];
    /* Valores de Stanford*/
    board[0][0] = 16.16;
    board[0][1] = -3.03;
    board[0][2] = 0.99;
    board[0][3] = 0.43;
    board[0][4] = 0.43;
    board[0][5] = 0.99;
    board[0][6] = -3.03;
    board[0][7] = 16.16;

    board[1][0] = -4.12;
    board[1][1] = -1.81;
    board[1][2] = -0.08;
    board[1][3] = -0.27;
    board[1][4] = -0.27;
    board[1][5] = -0.08;
    board[1][6] = -1.81;
    board[1][7] = -4.12;

    board[2][0] = 1.33;
    board[2][1] = -0.04;
    board[2][2] = 0.51;
    board[2][3] = 0.07;
    board[2][4] = 0.07;
    board[2][5] = 0.51;
    board[2][6] = -0.04;
    board[2][7] = 1.33;

    board[3][0] = 0.63;
    board[3][1] = -0.18;
    board[3][2] = -0.04;
    board[3][3] = -0.01;
    board[3][4] = -0.01;
    board[3][5] = -0.04;
    board[3][6] = -0.18;
    board[3][7] = 0.63;

    board[4][0] = 0.63;
    board[4][1] = -0.18;
    board[4][2] = -0.04;
    board[4][3] = -0.01;
    board[4][4] = -0.01;
    board[4][5] = -0.04;
    board[4][6] = -0.18;
    board[4][7] = 0.63;

    board[5][0] = 1.33;
    board[5][1] = -0.04;
    board[5][2] = 0.51;
    board[5][3] = 0.07;
    board[5][4] = 0.07;
    board[5][5] = 0.51;
    board[5][6] = -0.04;
    board[5][7] = 1.33;

    board[6][0] = -4.12;
    board[6][1] = -1.81;
    board[6][2] = -0.08;
    board[6][3] = -0.27;
    board[6][4] = -0.27;
    board[6][5] = -0.08;
    board[6][6] = -1.81;
    board[6][7] = -4.12;

    board[7][0] = 16.16;
    board[7][1] = -3.03;
    board[7][2] = 0.99;
    board[7][3] = 0.43;
    board[7][4] = 0.43;
    board[7][5] = 0.99;
    board[7][6] = -3.03;
    board[7][7] = 16.16;

    for (i = 0; i<SIZE; i++)
        for (j = 0; j<SIZE; j++){
            if (ai.grelha[i][j] == player)
                hvalue += board[i][j];
        }

    return hvalue;
}

//---------------------------------------Jogar AI-----------------------------------------------------------------------
double bestmoveAI(ESTADO ai){ ///----Cópia quase Total de bestmove---
    VALOR player, opponent;
    int i,j,bestmove_i = 0,bestmove_j = 0;
    double newscore,score;

    ///-----Variáveis da função Jogar ---------------
    int legalmoves[SIZE][SIZE],idelta = 0,jdelta = 0,x = 0,y = 0;

    ESTADO f = ai;

    /*Definir jogador e peça*/
    if (f.peca == VALOR_X){
        player = VALOR_X;
        opponent = VALOR_O;
    }else {
        player = VALOR_O;
        opponent = VALOR_X;
    }

    score = 0;
    newscore = 0;

    /*Copiar estado atual para current_board*/
    for (i = 0; i<SIZE; i++)
        for (j = 0; j<SIZE; j++){
            if(f.grelha[i][j] == VALOR_O)
                f.grelha[i][j] = VALOR_O;
            else if (f.grelha[i][j] == VALOR_X)
                f.grelha[i][j] = VALOR_X;
            else
                f.grelha[i][j] = VAZIA;
        }


    ///----------------------------------Cópia de Código da Função Jogar------------------------------------------------


    /*  Iniciar o array legalmoves a zeros    */
    for(i = 0; i < SIZE; i++)
        for(j = 0; j < SIZE; j++)
            legalmoves[i][j] = 0;

    for(i = 0; i < SIZE; i++)
        for(j = 0; j < SIZE; j++)
        {
            if(f.grelha[i][j] != VAZIA)     /* O quadrado está vazio ? Não, então prosseguimos*/
                continue;

            /* Verificar os quadrados à volta para encontrar uma peça do adversário*/
            for(idelta = -1; idelta <= 1; idelta++)
                for(jdelta = -1; jdelta <= 1; jdelta++)
                {
                    /*Não verificar fora do array*/
                    if(i + idelta < 0 || i + idelta >= SIZE ||
                       j + jdelta < 0 || j + jdelta >= SIZE ||
                       (idelta==0 && jdelta==0))
                        continue;
                    /* Verificando o quadrado agora*/
                    if(f.grelha[i + idelta][j + jdelta] == opponent)
                    {
                        /* Se encontrarmos uma peça do oponente, mover na direção delta
                         * Procurando uma peça do jogador a seguir da peça do oponente*/

                        x = i + idelta;
                        y = j + jdelta;             /* Mover para o quadrado do oponente*/

                        /*Procuar uma peça do jogador na direção delta*/
                        for(;;)
                        {
                            x += idelta;        /* Ir para o próximo */
                            y += jdelta;        /* quadrado na direção delta */

                            /* Se sairmos da grelha, desistir*/
                            if(x < 0 || x >= SIZE || y < 0 || y >= SIZE)
                                break;
                            /* Se encontrarmos um quadrado vazia, desistir*/
                            if(f.grelha[x][y] == VAZIA)
                                break;
                            /* Se encontarmos um quadrado com peça do jogador*
                             * TEMOS UMA JOGADA VÁLIDA !!*/
                            if(f.grelha[x][y] == player)
                            {
                                legalmoves[i][j] = 1;
                                break;
                            }
                        }
                    }
                }
        }

    ///----------------------------------Cópia de Código da Função Jogar------------------------------------------------




    /*Verificar todos as possiveis jogadas*/
    for (i = 0; i<SIZE; i++){
        f = ai;                              /*Dar reset a f após cada ciclo*/
        for (j = 0; j<SIZE; j++){
            if (legalmoves[i][j] == 1){     /*Se o movimento for legal*/
                f.grelha[i][j] = player;    /* Efetuar jogada*/
                f = tilestoflip(f,i,j);
                newscore = heuristic(f);            /* Virar as respetivas peças*/
            }                               /*Guardar pontuação*/
            if (score < newscore){         /*Se for maior que anterior, guardar o valor*/
                score = newscore;          /*Guardar coordenadas também*/
                bestmove_i = i;
                bestmove_j = j;
            }
        }
    }

    ai.grelha[bestmove_i][bestmove_j] = player;
    ai = tilestoflip(ai,bestmove_i,bestmove_j);
    score = heuristic(ai);
    ai.peca = opponent;


    return score;
}

ESTADO jogarAI(ESTADO ai){ ///--- Só existe para não haver output de jogada inválida ao dar bruteforce das jogadas
    VALOR player, opponent;
    int i = 0,j = 0,idelta = 0,jdelta = 0, x = 0,y = 0,bestmove_i = 0,bestmove_j = 0,lin = 0, col = 0,legalmoves[SIZE][SIZE];
    int legalmovestemp[SIZE][SIZE];
    double score, newscore;

    /*Declaração de peças*/
    if (ai.peca == VALOR_X){
        player = VALOR_X;
        opponent = VALOR_O;
    }else{
        player = VALOR_O;
        opponent = VALOR_X;
    }

    ai.peca = temp.peca;

    /*  Iniciar o array legalmoves a zeros    */
    for(i = 0; i < SIZE; i++)
        for(j = 0; j < SIZE; j++)
            legalmoves[i][j] = 0;

    for(i = 0; i < SIZE; i++)
        for(j = 0; j < SIZE; j++)
        {
            if(ai.grelha[i][j] != VAZIA)     /* O quadrado está vazio ? Não, então prosseguimos*/
                continue;

            /* Verificar os quadrados à volta para encontrar uma peça do adversário*/
            for(idelta = -1; idelta <= 1; idelta++)
                for(jdelta = -1; jdelta <= 1; jdelta++)
                {
                    /*Não verificar fora do array*/
                    if(i + idelta < 0 || i + idelta >= SIZE ||
                       j + jdelta < 0 || j + jdelta >= SIZE ||
                       (idelta==0 && jdelta==0))
                        continue;
                    /* Verificando o quadrado agora*/
                    if(ai.grelha[i + idelta][j + jdelta] == opponent)
                    {
                        /* Se encontrarmos uma peça do oponente, mover na direção delta
                         * Procurando uma peça do jogador a seguir da peça do oponente*/

                        x = i + idelta;
                        y = j + jdelta;             /* Mover para o quadrado do oponente*/

                        /*Procuar uma peça do jogador na direção delta*/
                        for(;;)
                        {
                            x += idelta;        /* Ir para o próximo */
                            y += jdelta;        /* quadrado na direção delta */

                            /* Se sairmos da grelha, desistir*/
                            if(x < 0 || x >= SIZE || y < 0 || y >= SIZE)
                                break;
                            /* Se encontrarmos um quadrado vazia, desistir*/
                            if(ai.grelha[x][y] == VAZIA)
                                break;
                            /* Se encontarmos um quadrado com peça do jogador*
                             * TEMOS UMA JOGADA VÁLIDA !!*/
                            if(ai.grelha[x][y] == player)
                            {
                                legalmoves[i][j] = 1;
                                break;
                            }
                        }
                    }
                }





        }




    /* Verificar todos as jogadas válidas*/
    for (lin = 0; i<SIZE; i++){
        temp = ai;
        for (col = 0; j<SIZE; j++)
        {
            if (legalmoves[lin][col] == 0)
                continue;
            /*Copiar a grelha de ai para temp*/
            for (i = 0; i<SIZE; i++)
                for (j = 0; j<SIZE; j++)
                    temp.grelha[i][j] = ai.grelha[i][j];

            temp.grelha[lin][col] = player;
            temp = tilestoflip(temp,lin,col);

            /*Encontrar jogadas para o humano após jogada do computador*/
            temp.peca = opponent;
            /*Declaração de peças*/
            if (ai.peca == VALOR_X){
                player = VALOR_X;
                opponent = VALOR_O;
            }else{
                player = VALOR_O;
                opponent = VALOR_X;
            }

            ///--------------------------------
            /*  Iniciar o array legalmoves a zeros    */
            for(i = 0; i < SIZE; i++)
                for(j = 0; j < SIZE; j++)
                    legalmoves[i][j] = 0;

            for(i = 0; i < SIZE; i++)
                for(j = 0; j < SIZE; j++)
                {
                    if(temp.grelha[i][j] != VAZIA)     /* O quadrado está vazio ? Não, então prosseguimos*/
                        continue;

                    /* Verificar os quadrados à volta para encontrar uma peça do adversário*/
                    for(idelta = -1; idelta <= 1; idelta++)
                        for(jdelta = -1; jdelta <= 1; jdelta++)
                        {
                            /*Não verificar fora do array*/
                            if(i + idelta < 0 || i + idelta >= SIZE ||
                               j + jdelta < 0 || j + jdelta >= SIZE ||
                               (idelta==0 && jdelta==0))
                                continue;
                            /* Verificando o quadrado agora*/
                            if(temp.grelha[i + idelta][j + jdelta] == opponent)
                            {
                                /* Se encontrarmos uma peça do oponente, mover na direção delta
                                 * Procurando uma peça do jogador a seguir da peça do oponente*/

                                x = i + idelta;
                                y = j + jdelta;             /* Mover para o quadrado do oponente*/

                                /*Procuar uma peça do jogador na direção delta*/
                                for(;;)
                                {
                                    x += idelta;        /* Ir para o próximo */
                                    y += jdelta;        /* quadrado na direção delta */

                                    /* Se sairmos da grelha, desistir*/
                                    if(x < 0 || x >= SIZE || y < 0 || y >= SIZE)
                                        break;
                                    /* Se encontrarmos um quadrado vazia, desistir*/
                                    if(temp.grelha[x][y] == VAZIA)
                                        break;
                                    /* Se encontarmos um quadrado com peça do jogador*
                                     * TEMOS UMA JOGADA VÁLIDA !!*/
                                    if(temp.grelha[x][y] == player)
                                    {
                                        legalmovestemp[i][j] = 1;
                                        break;
                                    }
                                }
                            }
                        }
                }

            if (legalmovestemp[i][j] == 1){
                temp.grelha[i][j] = player;
                temp = tilestoflip(temp,i,j);
            }


            newscore = heuristic(temp);

            if (newscore>score){
                score = newscore;
                bestmove_i = lin;
                bestmove_j = col;
            }

        }
    }

    ai.grelha[bestmove_i][bestmove_j] = player;
    ai = tilestoflip(ai,bestmove_i,bestmove_j);
    ai.peca = opponent;

    return ai;
}



