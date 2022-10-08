//
// Created by gepoo on 01/04/19.
//

#ifndef PROJ_INTERPRETADOR_H
#define PROJ_INTERPRETADOR_H

#include "estado.h"

//ESTADO é uma variavel

void printterminal(ESTADO e);
void interpretador(ESTADO e);
ESTADO tilestoflip(ESTADO e, int linjogada, int coljogada);
void escrever(ESTADO e);

ESTADO jogar(ESTADO e, int linjogada, int coljogada);
#endif //PROJ_INTERPRETADOR_H
