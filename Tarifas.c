//
//  Tarifas.c
//  ProjetoEstacionamento
//
//  Created by Gonçalo Henrique Viegas Bernardino on 05/12/2025.
//
#include "Tarifas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int guardartarifas(Tarifa tarifas[], int numtarifas){
    FILE *f = fopen("Tarifario.txt", "w");
    if(f==NULL){
        return 0;
    }
    for(int i=0 ; i<numtarifas; i++){
        fprintf(f, "%c\t%s\t%02d:%02d\t%02d:%02d\t%.2f\n", tarifas[i].tipo, tarifas[i].codigo, tarifas[i].horaInf, tarifas[i].minInf, tarifas[i].horaSup, tarifas[i].minSup,tarifas[i].valor);
    }
    fclose(f);
    return 1;
   
}

int modificatarifa(Tarifa tarifas[], int *numtarifas) {
    float valortemp;
    char Codigoescolhido[5];
    printf("Qual o código associado à tarifa que quer modificar (CT1, CT2,...)?\n");
    printf("Código: ");
    scanf("%s", Codigoescolhido);
    for(int i=0;i<*numtarifas; i++) {
        if(strcmp(tarifas[i].codigo, Codigoescolhido) == 0){
            printf("Valor atual do %s: %.2f\n", Codigoescolhido, tarifas[i].valor);
            do{
                printf("Novo valor: ");
                scanf("%f", &valortemp);
                if (valortemp<=0) {
                    printf("O novo valor deve ser maior que 0.00");
                }
            }while(valortemp<=0);
            tarifas[i].valor = valortemp;
            if (guardartarifas(tarifas, *numtarifas)) {
                printf("Valor modificado com sucesso.");
                return 1;
            }
        }
    }
    printf("O código %s não existe no tarifário.", Codigoescolhido);
    return 0;
}


void mostrarTabela(Tarifa tarifas[], int n) {
    printf("\n------------ VALORES DEFINIDOS ------------\n");
    for (int i = 0; i < n; i++) {
        printf("%s\tValor: %.2f €\t(%02d:%02d - %02d:%02d)\t%c\n",
               tarifas[i].codigo,
               tarifas[i].valor,
               tarifas[i].horaInf, tarifas[i].minInf,
               tarifas[i].horaSup, tarifas[i].minSup,
               tarifas[i].tipo);
    }
    printf("------------------------------------------\n");}

int lertarifas(Tarifa tarifas[], int *numtarifas) { //Ler o ficheiro criado "Tarifario.txt"
    // Validar parâmetros
    if (tarifas == NULL || numtarifas == NULL) {
        return 0;
    }
    printf("Tarifario.txt não encontrado.");
    
    FILE *f = fopen("Tarifario.txt", "r"); //abrir ficheiro para leitura
    
    if (f == NULL) {
        f = fopen("Tarifario.txt", "r");
        if (f == NULL) {
            printf("Não foi possível abrir o ficheiro após criação.\n");
            return 0;
        }
    }

    *numtarifas = 0;
    char linha[200];
    
    // Ler linha por linha
    while (fgets(linha, sizeof(linha), f) != NULL && *numtarifas < MAX_TARIFAS) {
        // Remover \n do final
        linha[strcspn(linha, "\n")] = 0;
        
        // Ignorar linhas vazias
        if (strlen(linha) == 0) {
            continue;
        }
        //variaveis temporararias
        char tipo;
        char codigo[10];
        int horaInf, minInf, horaSup, minSup;
        float valor;
        int resultado = sscanf(linha, "%c\t%[^\t]\t%d:%d\t%d:%d\t%f", &tipo, codigo, &horaInf, &minInf, &horaSup, &minSup, &valor);
        
        if (resultado != 7) {
           
            printf("Linha mal formatada: '%s'\n", linha);
            continue;
        }
        
        if (tipo != 'H' && tipo != 'D') {
            printf("Tipo '%c' inválido\n", tipo);
            continue;
        }
        
        if (valor <= 0) {
            printf("Valor %.2f inválido\n", valor);
            continue;
        }
        
        tarifas[*numtarifas].tipo = tipo;
        strcpy(tarifas[*numtarifas].codigo, codigo);
        tarifas[*numtarifas].horaInf = horaInf;
        tarifas[*numtarifas].minInf = minInf;
        tarifas[*numtarifas].horaSup = horaSup;
        tarifas[*numtarifas].minSup = minSup;
        tarifas[*numtarifas].valor = valor;
        
        (*numtarifas)++;
    }
    
    fclose(f);
    
    if (*numtarifas == 0) {
        printf("Nenhuma tarifa válida foi carregada\n");
        return 0;
    }
    
    printf("%d tarifa(s) carregada(s) com sucesso.\n", *numtarifas);
    return 1;
}


