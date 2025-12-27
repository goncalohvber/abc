#ifndef Estacionamento_h
#define Estacionamento_h
#include "Instalacao.h"
#include <stdio.h>

typedef struct estacionamento{
    int numE;
    char matricula[10];
    int anoE, mesE, diaE, horaE, minE, anoS, mesS, diaS, horaS, minS;
    char lugar[6];
} estacionamento;

void MostrarFicheiroEstacionamento(char *nomeFicheiro);
int personalizapp(Confparque config);

#endif /* Estacionamento_h */

