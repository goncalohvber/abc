#ifndef Estacionamento_h
#define Estacionamento_h
#include "Instalacao.h"
#include "Tarifas.h"
#include <stdio.h>

typedef struct estacionamento{
    int numE;
    char matricula[10];
    int anoE, mesE, diaE, horaE, minE, anoS, mesS, diaS, horaS, minS;
    char lugar[6];
} estacionamento;

typedef struct {
    char status; // 'X' = ocupado, '-' = livre
    char matricula[10]; // guardar matrícula do carro
} Lugar;

void MostrarFicheiroEstacionamento(char *nomeFicheiro);
int personalizapp(Confparque config);
void MostrarMapaOcupacao_ComMapa(Confparque config, char *ficheiroOcupacao, Lugar mapa[][MAX_FILAS][MAX_LUGARES]);
void InicializarMapa(Lugar mapa[][MAX_FILAS][MAX_LUGARES], Confparque config);
void PreencherMapaComOcupacoes(Lugar mapa[][MAX_FILAS][MAX_LUGARES], char *ficheiroOcupacao);
void DesenharMapa(Lugar mapa[][MAX_FILAS][MAX_LUGARES], Confparque config);

#endif /* Estacionamento_h */
