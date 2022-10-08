//
// Created by gepoo on 01/04/19.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <elf.h>
#include <stdbool.h>
#include "estado.h"
#include "Interpretador.h"
#include "AI.h"


//-----------------------------------------Criar Novo Jogo--------------------------------------------------------------
ESTADO novojogo(ESTADO e, char *peca){
    int i,j;
    for (i = 0; i<SIZE; i++)           /* set all array positions's value to '-'*/
        for (j = 0; j<SIZE; j++)
            e.grelha[i][j] = VAZIA;

    e.grelha[3][4] = VALOR_O;
    e.grelha[4][3] = VALOR_O;       /* set initial pieces  */
    e.grelha[3][3] = VALOR_X;
    e.grelha[4][4] = VALOR_X;

    printf("Novo Jogo !\n");


    switch (toupper(peca[0])){
        case 'X':
            e.peca = VALOR_X;
            break;
        case 'O':
            e.peca = VALOR_O;
            break;
        default:
            printf("A peça tem de ser X ou O (rtfm)\n");
    }
    return e;
}
//---------------------------------Escrever Estado do Jogo--------------------------------------------------------------
void escrever(ESTADO e){
    FILE *f;
    int i,j;

    /*Abre o estado.txt em modo write+*/
    f = fopen("estado.txt", "w+");

    for (i = 0; i<SIZE ; i++){
        if (i>0)
            fprintf(f,"%s","\n");                /*newline used for proper text positioning  */
        for (j = 0; j<SIZE; j++){
            if (e.grelha[i][j] == VALOR_O )      /* if "O" print 'O' if "X" print 'X' else print '-'*/
                fprintf(f, "%s", "O ");
            else if (e.grelha[i][j] == VALOR_X)  /*Print X */
                fprintf(f,"%s","X ");
            else
                fprintf(f,"%s","- ");             /*Print -*/
        }
    }
    fclose(f);

    printf("\nJogo guardado em estado.txt\n");
}
//--------------------------------Ler Estado do Jogo--------------------------------------------------------------------
ESTADO ler(ESTADO e){
    int i,j;
    FILE *f;
    /* Declarar de FILE stream, e o char onde vai ser impresso o estado de jogo*/
    char save[MAXBUFFER][MAXBUFFER];

    /*Abrir estado.txt*/
    f = fopen("estado.txt", "r");

    /* Ler estado e imprimir no char save*/
    for (i = 0 ; i<SIZE; i++)
        for (j = 0; j<SIZE; j++)
            fscanf(f,"%s", &save[i][j]);

    /* Alterar o ESTADO e conforme o ficheiro*/
    for (i = 0 ; i<SIZE; i++)
        for (j = 0; j<SIZE; j++){
            if (save[i][j] == 'X')
                e.grelha[i][j] = VALOR_X;
            if (save[i][j] == 'O')
                e.grelha[i][j] = VALOR_O;
            if (save[i][j] == '-')
                e.grelha[i][j] = VAZIA;
        }

    fclose(f);
    printf("\nEstado de jogo lido!\n");
    return e;
}
//-------------------------------Retroceder(Undo) o Estado do Jogo------------------------------------------------------
ESTADO push(ESTADO e){
    int i,j;
    //Criar um novo node temp e alocar memoria
    struct Node* temp;
    temp = (struct Node*)malloc(sizeof(struct Node));

    //verificar se a stack esta cheia
    if(!temp){
        printf("A stack está sobrelotada!\n");
    }

    //Introduzir os dados no respetivo campo em temp
    for (i = 0; i<SIZE; i++)
        for (j = 0; j<SIZE; j++)
            temp->grelha[i][j] = e.grelha[i][j];

    temp->peca = e.peca;
    //Direcionar o apontador top para temp
    temp->next = top;
    //Tornar temp o topo da stack
    top = temp;

    return e;
}

ESTADO pop(){
    int i,j;
    struct Node* temp;


    /*Verificar se está vazia*/
    if(top == NULL)
        printf("Não há jogadas para desfazer!\n");
    else {
        //Top fica com o valor de temp
        temp = top;

        //O node seguinte fica a ser o topo
        top = top->next;

        //liberta memoria do nodo de topo
        free(temp);
    }
}

ESTADO transfer(ESTADO e){
    int i,j;

    if(top == NULL){
        printf("Não há jogadas para transferir!\n");
    }else{
        e.peca = top->peca;

        for (i = 0; i<SIZE; i++)
            for (j = 0; j<SIZE;j++)
                e.grelha[i][j] = top->grelha[i][j];
    }
    return e;
}

ESTADO printlista(){
    int i,j;
    char c = ' ';

    if(top == NULL){
        printf("Não há jogadas para desfazer!\n");
    }else {
        for (i = 0; i < SIZE; i++) {
            for (j = 0; j < SIZE; j++) {
                switch (top->grelha[i][j]) {
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
                }
                printf("%c ", c);
            }
            printf("\n");
        }
        printf("\n");
    }

} ///----Apagar depois------ meramente para teste----------

//----------------------------------Execução e Validação de Jogadas-----------------------------------------------------
ESTADO tilestoflip(ESTADO e, int linjogada, int coljogada){
    VALOR player, opponent; int x = 0,y = 0,lindelta = 0,coldelta = 0;

    /* Declaração de Peças*/
    if (e.peca == VALOR_X){
        player = VALOR_X;
        opponent = VALOR_O;
    }else {
        player = VALOR_O;
        opponent = VALOR_X;
    }

    /* Procurar peça do adversário no quadrado que rodeia  */
    for (lindelta = -1; lindelta <=1; lindelta++)
        for (coldelta = -1; coldelta<=1; coldelta++)
        {
            /*Não verificar fora do tabuleiro ou dentro do quadrado atual*/
            if (linjogada + lindelta < 0 || linjogada + lindelta >=SIZE ||
                coljogada + coldelta < 0 || coljogada + coldelta >=SIZE ||(lindelta == 0 && coldelta == 0))
                continue;
            /* Verifica-se o quadrado agora*/
            if (e.grelha[linjogada+lindelta][coljogada+coldelta] == opponent)
            {
                /*Se encontrada uma peça do adversário, ir na mesma direção à procura de mais*/
                x = linjogada + lindelta;
                y = coljogada + coldelta;        /* Avançar para o  quadrado do adversário */

                for (;;)
                {
                    x += lindelta;     /* Avançar para o quadrado seguinte*/
                    y += coldelta;
                    /*Se sairmos do tabuleiro, desistir*/
                    if (x < 0||x >=SIZE||y < 0|| y>=SIZE)
                        break;
                    /*Se o quadrado está vaziom, desistir*/
                    if (e.grelha[x][y] == VAZIA)
                        break;
                    /*Se encontrada uma peça do jogador, voltar para trás a partir daqui
                     *  Mudando todas as peças do adversário para peças do jogador*/
                    if (e.grelha[x][y] == player)
                    {
                        while(e.grelha[x -= lindelta][y -= coldelta] == opponent) /*Adversário ?*/
                            e.grelha[x][y] = player;        /*Sim, mudar peça*/
                        break;                              /* Acabou*/
                    }

                }
            }
        }

    return e;
}

ESTADO fim(ESTADO e){
    int i,j,pontos_x,pontos_o, countX = 0, countO = 0,countVAZIA = 0;

    for (i = 0; i<SIZE; i++)
        for (j = 0; j<SIZE; j++){
            if (e.grelha[i][j] == VALOR_O)
                ++countO;
            else if (e.grelha[i][j] == VALOR_X)
                ++countX;
            else
                ++countVAZIA;
        }

    if (countVAZIA == 0){
        for (i = 0; i<SIZE; i++)
            for (j = 0; j<SIZE; j++){
                if (e.grelha[i][j] = VALOR_X)
                    ++pontos_x;
                else
                    ++pontos_o;
            }
        if (pontos_o > pontos_x)
            printf("\nGanhou o jogador 'O' com %d peças. O perdedor acabou com %d peças\n" ,countO,countX);
        else if (pontos_o == pontos_x)
            printf("\n Impssível, Empate !\n");
        else
            printf("\nGanhou o jogador 'O' com %d peças. O perdedor acabou com %d peças\n" ,countX,countO);

        exit(1);
    }

}

ESTADO jogar(ESTADO e, int linjogada, int coljogada){
    VALOR player, opponent; int i = 0,j = 0,idelta = 0,jdelta = 0,legalmoves[SIZE][SIZE],x = 0,y = 0;

    /*Declaração de peças*/
    if (e.peca == VALOR_X){
        player = VALOR_X;
        opponent = VALOR_O;
    }else{
        player = VALOR_O;
        opponent = VALOR_X;
    }

    ///--------------------------------Início de Validação--------------------------------------------------------------

    /*  Iniciar o array legalmoves a zeros    */
    for(i = 0; i < SIZE; i++)
        for(j = 0; j < SIZE; j++)
            legalmoves[i][j] = 0;

    for(i = 0; i < SIZE; i++)
        for(j = 0; j < SIZE; j++)
        {
            if(e.grelha[i][j] != VAZIA)     /* O quadrado está vazio ? Não, então prosseguimos*/
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
                    if(e.grelha[i + idelta][j + jdelta] == opponent)
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
                            if(e.grelha[x][y] == VAZIA)
                                break;
                            /* Se encontarmos um quadrado com peça do jogador*
                             * TEMOS UMA JOGADA VÁLIDA !!*/
                            if(e.grelha[x][y] == player)
                            {
                                legalmoves[i][j] = 1;
                                break;
                            }
                        }
                    }
                }
        }
    ///----------------------------------------Fim de Validação---------------------------------------------------------

    /*Execução de jogada e troca de peça*/
    if (legalmoves[linjogada][coljogada] == 1){
        e.grelha[linjogada][coljogada] = player;
        e = tilestoflip(e,linjogada,coljogada);
        e.peca = opponent;
    }else if(!legalmoves){
        e.peca = opponent;
    }
    else{
        printf("\nJogada é inválida!\n");
    }
    return e;
}


//---------------------------------Impressão de Jogadas Legais----------------------------------------------------------
ESTADO legal(ESTADO e){
    VALOR player, opponent; int i = 0,j = 0,idelta = 0,jdelta = 0,legalmoves[SIZE][SIZE],x = 0,y = 0;

    /*Declaração de peças*/
    if (e.peca == VALOR_X){
        player = VALOR_X;
        opponent = VALOR_O;
    }else{
        player = VALOR_O;
        opponent = VALOR_X;
    }

    ///--------------------------------Início de Validação--------------------------------------------------------------

    /*  Iniciar o array legalmoves a zeros    */
    for(i = 0; i < SIZE; i++)
        for(j = 0; j < SIZE; j++)
            legalmoves[i][j] = 0;

    for(i = 0; i < SIZE; i++)
        for(j = 0; j < SIZE; j++)
        {
            if(e.grelha[i][j] != VAZIA)     /* O quadrado está vazio ? Não, então prosseguimos*/
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
                    if(e.grelha[i + idelta][j + jdelta] == opponent)
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
                            if(e.grelha[x][y] == VAZIA)
                                break;
                            /* Se encontarmos um quadrado com peça do jogador*
                             * TEMOS UMA JOGADA VÁLIDA !!*/
                            if(e.grelha[x][y] == player)
                            {
                                legalmoves[i][j] = 1;
                                break;
                            }
                        }
                    }
                }
        }


    /* Mudança dos valores de e.grelha para posteriormente serem mudados.*/
    for (i = 0; i<SIZE; i++){
        for (j = 0; j<SIZE; j++){
            if (legalmoves[i][j] == 1)
                e.grelha[i][j] = POSSIVEL;
        }
    }

    return e;
}

ESTADO retirarlegais(ESTADO e){
    int i,j;

    for (i = 0; i<SIZE; i++)
        for (j = 0; j<SIZE; j++)
            if (e.grelha[i][j] == POSSIVEL)
                e.grelha[i][j] = VAZIA;

    return e;
} ///--------------------Para retirar os '.' do estado--------

//---------------------------------------Melhor Jogada------------------------------------------------------------------
int points(ESTADO e){
    VALOR player, opponent; int i,j,score = 0;

    if (e.peca == VALOR_X){
        player = VALOR_X;
        opponent = VALOR_O;
    }else {
        player = VALOR_O;
        opponent = VALOR_X;
    }

    for (i = 0; i<SIZE;i++)
        for (j = 0; j<SIZE; j++){
            if (e.grelha[i][j] == opponent)
                --score;
            if (e.grelha[i][j] == player)
                ++score;
        }

    return score;
}

ESTADO bestmove(ESTADO e){
    VALOR player, opponent;
    int i,j,bestmove_i = 0,bestmove_j = 0;
    double newscore,score;

    ///-----Variáveis da função Jogar ---------------
    int legalmoves[SIZE][SIZE],idelta = 0,jdelta = 0,x = 0,y = 0;

    ESTADO f = e;

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
        f = e;                              /*Dar reset a f após cada ciclo*/
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
    e.grelha[bestmove_i][bestmove_j] = MELHOR;

    return e;
}

ESTADO retirarbest(ESTADO e){
    int i,j;

    for (i = 0; i<SIZE; i++)
        for (j = 0; j<SIZE; j++)
            if (e.grelha[i][j] == MELHOR)
                e.grelha[i][j] = VAZIA;

    return e;
} ///------------------Para retirar os '?' do estado------------

//---------------------------------------Interpretador------------------------------------------------------------------
ESTADO interpretar(ESTADO e, char *linha){
    char peca[MAXBUFFER];           /* Peça a ser usada X ou O      */
    char terminal[MAXBUFFER];       /* terminal para input          */
    int linjogada;                  /* Jogada a Fazer    */
    int coljogada;
    int ai_dificulty;

    int n;
    double score;
    sscanf(linha, "%s", terminal);

    /* toupper -> to upper */
    switch (toupper(terminal[0]))
    {
        case 'N':
            n = sscanf(linha, "%s %s", terminal, peca);
            if (n == 2){
                e = novojogo(e, peca);
                e = push(e);
                e.modo = 0;
            }
            else
                printf("Erro falta um parâmetro !\n");
            break;
        case 'L':
            sscanf(linha, "%s", terminal);
            e = ler(e);
            break;
        case 'E':
            sscanf(linha, "%s", terminal);
            escrever(e);
            break;
        case 'J':
            n = sscanf(linha,"%s %d %d", terminal, &linjogada,&coljogada);
            e = retirarlegais(e); /* Necessário para retirar os '.' de e.grelha*/
            e = retirarbest(e);   /* Necessário para retirar os '?' de e.grelha*/
            if (n == 3 && (coljogada>0 && coljogada<9) && (linjogada>0 && linjogada<9)){
                --linjogada;
                --coljogada;
                if (e.modo  == 1){
                    e = jogar(e,linjogada,coljogada);
                    e = jogarAI(e);
                }else
                    e = jogar(e,linjogada,coljogada);
                push(e);
                fim(e);
            }
            else
                printf("Erro! parâmetros incorretos!\n");
            break;
        case 'S':
            sscanf(linha, "%s", terminal);
            e = retirarbest(e); /* Remover '?' antes de apresentar jogadas legais '.'*/
            e = legal(e);
            break;
        case 'H':
            sscanf(linha, "%s", terminal);
            e = retirarlegais(e); /* Remover '.' antes de apresentar sugestão de jogada '?'*/
            e = bestmove(e);
            break;
        case 'U':
            sscanf(linha, "%s",terminal);
            pop();
            e = transfer(e);
            break;
        case 'A':
            n = sscanf(linha,"%s %s", terminal, peca);
            if (n == 2){
                if (peca[0] == 'X' || peca[0] == 'x'){
                    e = novojogoAI(e, peca);
                    e = jogarAI(e);
                }else
                    e = novojogoAI(e, peca);

            }else
                printf("Erro falta um parâmetro !\n");
            e.modo = 1;
            break;
        case 'T':
            score = getScore(e);
            e = jogarAI(e);
            printf("A pontuação é %f" ,score);
            break;
        case 'Q':
            printf("\nAdeus...");
            exit(1);
        default:
            printf("Comando não reconhecido(rtfm)!\n");
    }
    return e;
}


void printterminal(ESTADO e){
    switch (e.peca){
        case VALOR_X:
            printf("reversi X$");
            break;
        case VALOR_O:
            printf("reversi O$");
            break;
        default:
            printf("Reversi ?$ ");

    }

}

void interpretador(ESTADO e){
    char linha[MAXBUFFER];

    printterminal(e);

    while (fgets(linha, MAXBUFFER, stdin)){
        //printf("Vou interpretar a linha : %s\n" ,linha);
        e = interpretar(e,linha);

        printa(e);
        printterminal(e);
    }
}