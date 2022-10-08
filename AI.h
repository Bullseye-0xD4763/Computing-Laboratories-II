//
// Created by gepoo on 20/05/19.
//

#ifndef PROJ_AI_H
#define PROJ_AI_H

#include "estado.h"
#include "Interpretador.h"


ESTADO novojogoAI(ESTADO ai, char *peca);
ESTADO defplayer(ESTADO ai);
ESTADO legalAI(ESTADO ai);
double getScore(ESTADO ai);
double heuristic(ESTADO ai);
double bestmoveAI(ESTADO ai);
ESTADO tilestoflipAI(ESTADO ai, int linjogada, int coljogada); ///-----Necessária para jogar
ESTADO jogarAI(ESTADO temp);




struct AI_Node{
    ESTADO ai;
    struct AI_Node *left;
    struct AI_Node *right;
}AI_Node;


#endif //PROJ_AI_H
