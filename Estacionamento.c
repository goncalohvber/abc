//  Estacionamento.c
//  ProjetoEstacionamento
//
//  Created by Gonçalo Henrique Viegas Bernardino on 11/12/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include "Estacionamento.h"
int validaData(int dia, int mes, int ano);
int validamatricula(char *mat);
int validaLugar(char *lugar, int maxPisos, char maxFila, int maxLugares);
void LimpaFicheiroEstac(char *ficheirobase, char *ficheirovalido, char *ficheiroerros, Confparque config) {
    

        FILE *f_og = fopen(ficheirobase, "r");      // Ler o original 
        FILE *f_val = fopen(ficheirovalido, "w");   // Escrever o limpo
        FILE *f_err = fopen(ficheiroerros, "w");     // Escrever os erros


        if (f_og == NULL || f_val == NULL || f_err == NULL) {
            printf("Erro critico ao abrir ficheiros.\n");
            return;
        }

        estacionamento E;
        int novoID = 1;
        while (fscanf(f_og, "%d %s %d %d %d %d %d %s %d %d %d %d %d",
                      &E.numE, E.matricula,
                      &E.anoE, &E.mesE, &E.diaE, &E.horaE, &E.minE,
                      E.lugar,
                      &E.anoS, &E.mesS, &E.diaS, &E.horaS, &E.minS) == 13)
        {
            char letraConvertida = 'A' + (config.numfilas - 1);
            int entradaValida = validaData(E.diaE, E.mesE, E.anoE);
            int saidaValida = validaData(E.diaS, E.mesS, E.anoS);
            int matriculaValida = validamatricula(E.matricula);
            int lugarvalido = validaLugar(E.lugar, config.numpisos,letraConvertida, config.numlugares);
            if (entradaValida == 1 && saidaValida == 1 && matriculaValida == 1 && lugarvalido == 1) {
                
                fprintf(f_val, "%d %s %d %d %d %d %d %s %d %d %d %d %d\n",
                        novoID, E.matricula,
                        E.anoE, E.mesE, E.diaE, E.horaE, E.minE,
                        E.lugar,
                        E.anoS, E.mesS, E.diaS, E.horaS, E.minS);
                novoID++;
            }
            else {
                fprintf(f_err, "[ERRO DATA] Linha %d | Mat: %s | Entrada: %d/%d/%d | Saida: %d/%d/%d\n",
                        E.numE, E.matricula,
                        E.diaE, E.mesE, E.anoE,
                        E.diaS, E.mesS, E.anoS);
            }
        }

        fclose(f_og);
        fclose(f_val);
        fclose(f_err);
    }

void MostrarFicheiroEstacionamento(char *nomeFicheiro)
{
    FILE *f = fopen(nomeFicheiro, "r");
    
    if (f == NULL) {
        printf("Erro: Nao foi possivel abrir o ficheiro %s\n", nomeFicheiro);
        return;
    }

    estacionamento E; // Variável temporária para guardar a linha atual
    
    printf("\n--- REGISTO DE ESTACIONAMENTOS ---\n");

    // O ciclo while funciona como um 'for' que corre enquanto houver linhas para ler
    // O fscanf retorna o número de variáveis lidas
    while (fscanf(f, "%d %s %d %d %d %d %d %s %d %d %d %d %d",
                  &E.numE,
                  E.matricula,
                  &E.anoE, &E.mesE, &E.diaE, &E.horaE, &E.minE, // Data Entrada
                  E.lugar,
                  &E.anoS, &E.mesS, &E.diaS, &E.horaS, &E.minS  // Data Saida
                  ) == 13)
    {
        printf("Num: %-4d | Mat: %-8s | Lugar: %-4s | Ent: %02d/%02d/%d %02d:%02d | Sai: %02d/%02d/%d %02d:%02d\n",
               E.numE,
               E.matricula,
               E.lugar,
               E.diaE, E.mesE, E.anoE, E.horaE, E.minE,
               E.diaS, E.mesS, E.anoS, E.horaS, E.minS
               );
    }

    fclose(f);
}


void registentrada() {
    
}

