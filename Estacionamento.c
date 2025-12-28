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
int validaEantesS(int diaE, int mesE, int anoE, int horaE, int minE, int diaS, int mesS, int anoS, int horaS, int minS);
int ComparaDatas(int d1, int m1, int a1, int h1, int min1, int d2, int m2, int a2, int h2, int min2);
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
            int tempovalido = validaEantesS(E.diaE, E.mesE, E.anoE, E.horaE, E.minE, E.diaS, E.mesS, E.anoS, E.horaS, E.minS);
            if (entradaValida == 1 && saidaValida == 1 && matriculaValida == 1 && lugarvalido == 1 && tempovalido == 1) {
                
                fprintf(f_val, "%d %s %d %d %d %d %d %s %d %d %d %d %d\n",
                        novoID, E.matricula,
                        E.anoE, E.mesE, E.diaE, E.horaE, E.minE,
                        E.lugar,
                        E.anoS, E.mesS, E.diaS, E.horaS, E.minS);
                novoID++;
            }
            else if(entradaValida != 1) {
                fprintf(f_err, "[ERRO] Linha %d | Entrada: %d/%d/%d (Data de entrada não existe)\n", E.numE, E.diaE, E.mesE, E.anoE);
                
            }
            else if(saidaValida != 1){
                fprintf(f_err, "[ERRO] Linha %d | Entrada: %d/%d/%d (Data de saída não existe)\n", E.numE, E.diaS, E.mesS, E.anoS);
            }
            
            else if((matriculaValida != 1)){
                fprintf(f_err, "[ERRO] Linha %d | Matricula: %s (Matricula não valida)\n", E.numE, E.matricula);
            }
            
            else if(lugarvalido != 1){
                fprintf(f_err, "[ERRO] Linha %d | Lugar: %s (Lugar não compativel com o tamanho do estacionamento)\n",
                        E.numE, E.lugar);
            }
            
            else if(tempovalido != 1){
                fprintf(f_err, "[ERRO] Linha %d | A Data de entrada é posterior à Data de saida. (Impossível)\n", E.numE);
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

void gerarficheiroocupacao(char *ficheirovalido, char *ficheiroocupacao,
                           int diaU, int mesU, int anoU,int horaU,int minU) {
    FILE *f_val = fopen(ficheirovalido, "r");
    FILE *f_ocup = fopen(ficheiroocupacao, "w");
    
    if (f_val == NULL || f_ocup == NULL) {
        printf("Erro ao abrir ficheiros para gerar ocupacao.\n");
        return; }
        
    estacionamento E;
    int novonum=1;
    while (fscanf(f_val, "%d %s %d %d %d %d %d %s %d %d %d %d %d",
                  &E.numE, E.matricula,
                  &E.anoE, &E.mesE, &E.diaE, &E.horaE, &E.minE,
                  E.lugar,
                  &E.anoS, &E.mesS, &E.diaS, &E.horaS, &E.minS) == 13) {
        
        int entrouAntesOuIgual = ComparaDatas(E.diaE, E.mesE, E.anoE, E.horaE, E.minE, diaU, mesU, anoU, horaU, minU) <= 0;
        int saiuDepoisOuIgual  = ComparaDatas(E.diaS, E.mesS, E.anoS, E.horaS, E.minS, diaU, mesU, anoU, horaU, mesU) >= 0;

        if (entrouAntesOuIgual && saiuDepoisOuIgual) {
            fprintf(f_ocup, "%d %s %d %d %d %d %d %s %d %d %d %d %d\n",
                    novonum, E.matricula,
                    E.anoE, E.mesE, E.diaE, E.horaE, E.minE,
                    E.lugar,
                    E.anoS, E.mesS, E.diaS, E.horaS, E.minS);
            novonum++;
        }
    }
    fclose(f_val);
    fclose(f_ocup);
}


void registentrada(int a) {
    
}

