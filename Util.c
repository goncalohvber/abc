//
//  Util.c
//  ProjetoEstacionamento
//
//  Created by Gonçalo Henrique Viegas Bernardino on 11/12/2025.
//
#include "Estacionamento.h"
#include "Util.h"
#include "Instalacao.h"
#include <stdio.h>
#include <stdlib.h>
int personalizapp(Confparque config);
int ABissexto(int ano){
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
            return 1;
        }
        return 0;
    }

int validaData(int dia, int mes, int ano) {
    if (ano < 0 || mes < 1 || mes > 12) {
        return 0;
    }

    int diasnomes;
    if (mes == 2) {
        if (ABissexto(ano)) {
            diasnomes = 29;
        } else {
            diasnomes = 28;
        }
    } else if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        diasnomes = 30;
    } else {
        diasnomes = 31;
    }

    if (dia >= 1 && dia <= diasnomes) {
        return 1;
    } else {
        return 0;
    }
}

void mostrarMensagem(char *mens){
    printf("\n%s", mens);
    printf("\nDigite <Enter> para continuar... ");
    getchar();
    getchar();
}


void mostrarMenu(void) {
    int op1;
    printf("\n");
    printf("\t========================================\n");
    printf("\t||           GESTÃO DE PARQUE         ||\n");
    printf("\t========================================\n");
    printf("\t||                                    ||\n");
    printf("\t||   [1] Registar Entrada             ||\n");
    printf("\t||   [2] Registar Saída               ||\n");
    printf("\t||   [3] Mostrar Mapa                 ||\n");
    printf("\t||   [4] Reconfigurar Parque          ||\n");
    printf("\t||                                    ||\n");
    printf("\t||   ------------------------------   ||\n");
    printf("\t||   [0] Sair                         ||\n");
    printf("\t||                                    ||\n");
    printf("\t========================================\n");
    printf("\n\t>> Escolha uma opção: ");
    scanf("%d", &op1);
    
    switch (op1) {
        case 4:
            remove("configfeita.txt");
            Confparque config;
            system("clear");
            personalizapp(config);
    }

}
