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
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
int personalizapp(Confparque config);
void LimpaFicheiroEstac(char *ficheirobase, char *ficheirovalido, char *ficheiroerros, Confparque config);
int lerconfig(Confparque *config);
void gerarficheiroocupacao(char *ficheirovalido, char *ficheiroocupacao,
                           int diaU, int mesU, int anoU,int horaU,int minU);
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

int validamatricula(char *mat) {
        // Verificar o tamanho
        // Se não tiver exatamente 8 caracteres (ex: "AA-00-AA"), falha logo.
        if (strlen(mat) != 8) {
            return 0;
        }

        // Verificar os separadores
        // As posições 2 e 5 devem ser o traço
        if (mat[2] != '-' || mat[5] != '-') {
            return 0;
        }

        //Verificar os caracteres alfanuméricos
        for (int i = 0; i < 8; i++) {
            if (i == 2 || i == 5) { //para ignorar os hifens
                continue;
            }

            if (!isalnum(mat[i])) {
                return 0;
            }
        }
        //passou por tudo, valida
        return 1;
    }

int validaLugar(char *lugar, int maxPisos, int maxFila, int maxLugares) {
    if (strlen(lugar) < 3){
        return 0; }
    
    //PISO
    int piso = lugar[0] - '0';
    if (piso < 1 || piso > 5){
        return 0;
    } // Limite da estrutura
    if (piso > maxPisos) {
        return 0;
    }    // Limite definido pelo utilizador
    
    //FILA
    char fila = lugar[1];
    if (fila < 'A' || fila > 'Z') {
        return 0;
    }
    if (fila > maxFila){
        return 0;
    }
    
    //NUMERO
    int numero = atoi(&lugar[2]); // atoi da biblioteca stdlib, serve para depois do lugar[2] ele tbm considerar os restantes lugares que sejam também números (necessário porque o lugar pode ter 1 digito ou 2)
    if (numero < 1) {
        return 0;
    }
    if (numero > maxLugares){
        return 0;
    }
    
    //se o resto for valido:
    return 1;
}

int validaEantesS(int diaE, int mesE, int anoE, int horaE, int minE, int diaS, int mesS, int anoS, int horaS, int minS) { //função que garante o carro sai, depois de entrar)
    if(anoE>anoS)
        return 0;
    if(anoS>anoE)
        return 1;
    
    //Se o código chegar aqui, os anos são iguais (o mesmo aplica-se aos meses, dias e horas, que vao seguir o mesmo formato
    
    if(mesE>mesS)
        return 0;
    if(mesS>mesE)
        return 1;
    
    if(diaE>diaS)
        return 0;
    if(diaS>diaE)
        return 1;
    
    if(horaE>horaS)
        return 0;
    if(horaS>horaE)
        return 1;
    
    if(minS>minE)
        return 1;
    
    return 0;
}

void mostrarMensagem(char *mens){
    printf("\n%s", mens);
    printf("\nDigite <Enter> para continuar... ");
    getchar();
    getchar();
}


void mostrarMenu(void) {
    int op1;
    int diaU, mesU, anoU, horaU, minU;
    Confparque config;
    if (!lerconfig(&config)) {
        printf("Erro ao carregar configuração!\n");
        return; }
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
                
            case 1:
                if (registarEntrada(config, "estacionamentos.txt")) {
                    printf("\n Operação concluída!\n");
                } else {
                    printf("\nFalha ao registar entrada!\n");
                }
                mostrarMensagem("Prima Enter para continuar...");
                mostrarMenu();
                break;
                
            case 2:
                if (registarSaida(config, "estacionamentos.txt")) {
                    printf("\nOperação concluída!\n");
                } else {
                    printf("\nFalha ao registar saída!\n");
                }
                mostrarMensagem("Prima Enter para continuar...");
                mostrarMenu();
                LimpaFicheiroEstac("estacionamentos.txt", "estacionamentos_validos.txt", "relatorio_erros.txt", config);
                break;
                
            case 3:
                printf("Qual a data que quer ver? (DD MM AAAA): ");
                scanf("%d %d %d", &diaU, &mesU, &anoU);
                printf("Qual a hora? (HH MM): ");
                scanf("%d %d", &horaU, &minU);
                
                gerarficheiroocupacao("estacionamentos_validos.txt", "Ocupacaoatual.txt",
                                     diaU, mesU, anoU, horaU, minU);
                
                Lugar mapa[MAX_PISOS][MAX_FILAS][MAX_LUGARES];
                MostrarMapaOcupacao_ComMapa(config, "Ocupacaoatual.txt", mapa);
                
                mostrarMensagem("\nPrima Enter para continuar...");
                break;
                
            case 4:
                remove("configfeita.txt");
                Confparque config;
                system("clear");
                personalizapp(config);
                break;
        }
        
    }



int ComparaDatas(int d1, int m1, int a1, int h1, int min1,
                 int d2, int m2, int a2, int h2, int min2) {
    
    if (a1 > a2) return 1;
    if (a1 < a2) return -1;
    
    if (m1 > m2) return 1;
    if (m1 < m2) return -1;

    if (d1 > d2) return 1;
    if (d1 < d2) return -1;

    if (h1 > h2) return 1;
    if (h1 < h2) return -1;

    if (min1 > min2) return 1;
    if (min1 < min2) return -1;

    return 0;
}


