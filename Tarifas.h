#ifndef Tarifas_h
#define Tarifas_h
#define MAX_TARIFAS 10
#include <stdio.h>

typedef struct tarifa{
    char tipo;          // 'H' ou 'D'
    char codigo[10];    // "CT1", "CT2", etc
    int horaInf;        // 0-23
    int minInf;         // 0-59
    int horaSup;        // 0-23
    int minSup;         // 0-59
    float valor;    // Valor em euros
} Tarifa;


#endif /* Tarifas_h */
