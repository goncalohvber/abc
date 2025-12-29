//  Estacionamento.c
//  ProjetoEstacionamento
//
//  Created by Gonçalo Henrique Viegas Bernardino on 11/12/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include "Estacionamento.h"
#include "Tarifas.h"
int validaData(int dia, int mes, int ano);
int validamatricula(char *mat);
int validaLugar(char *lugar, int maxPisos, char maxFila, int maxLugares);
int validaEantesS(int diaE, int mesE, int anoE, int horaE, int minE, int diaS, int mesS, int anoS, int horaS, int minS);
int ComparaDatas(int d1, int m1, int a1, int h1, int min1, int d2, int m2, int a2, int h2, int min2);
float CalcularPreco(int dE, int mE, int aE, int hE, int minE, int dS, int mS, int aS, int hS, int minS, Tarifa tarifas[], int numTarifas);
int lertarifas(Tarifa tarifas[], int *numtarifas);

void LimpaFicheiroEstac(char *ficheirobase, char *ficheirovalido, char *ficheiroerros, Confparque config) {
    
    FILE *f_og = fopen(ficheirobase, "r");
    FILE *f_val = fopen(ficheirovalido, "w");
    FILE *f_err = fopen(ficheiroerros, "w");

    if (f_og == NULL || f_val == NULL || f_err == NULL) {
        printf("Erro critico ao abrir ficheiros.\n");
        return;
    }

    estacionamento E;
    int novoID = 1;
    
    // ✅ Load tarifas BEFORE processing entries
    Tarifa tarifas[MAX_TARIFAS];
    int numTarifas = 0;
    
    if (!lertarifas(tarifas, &numTarifas)) {
        printf("ERRO: Não foi possível carregar as tarifas!\n");
        fclose(f_og);
        fclose(f_val);
        fclose(f_err);
        return;
    }
    
    printf("DEBUG: %d tarifas carregadas\n", numTarifas);
    
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
        int lugarvalido = validaLugar(E.lugar, config.numpisos, letraConvertida, config.numlugares);
        int tempovalido = validaEantesS(E.diaE, E.mesE, E.anoE, E.horaE, E.minE,
                                        E.diaS, E.mesS, E.anoS, E.horaS, E.minS);
        
        if (entradaValida == 1 && saidaValida == 1 && matriculaValida == 1 &&
            lugarvalido == 1 && tempovalido == 1) {
            
            float precoPagar = CalcularPreco(E.diaE, E.mesE, E.anoE, E.horaE, E.minE,
                                           E.diaS, E.mesS, E.anoS, E.horaS, E.minS,
                                           tarifas, numTarifas);
            
            fprintf(f_val, "%d %s %d %d %d %d %d %s %d %d %d %d %d %.2f\n",
                    novoID, E.matricula,
                    E.anoE, E.mesE, E.diaE, E.horaE, E.minE,
                    E.lugar,
                    E.anoS, E.mesS, E.diaS, E.horaS, E.minS,
                    precoPagar);
            novoID++;
        }
        else if(entradaValida != 1) {
            fprintf(f_err, "[ERRO] Linha %d | Entrada: %d/%d/%d (Data de entrada não existe)\n",
                    E.numE, E.diaE, E.mesE, E.anoE);
        }
        else if(saidaValida != 1){
            fprintf(f_err, "[ERRO] Linha %d | Entrada: %d/%d/%d (Data de saída não existe)\n",
                    E.numE, E.diaS, E.mesS, E.anoS);
        }
        else if(matriculaValida != 1){
            fprintf(f_err, "[ERRO] Linha %d | Matricula: %s (Matricula não valida)\n",
                    E.numE, E.matricula);
        }
        else if(lugarvalido != 1){
            fprintf(f_err, "[ERRO] Linha %d | Lugar: %s (Lugar não compativel com o tamanho do estacionamento)\n",
                    E.numE, E.lugar);
        }
        else if(tempovalido != 1){
            fprintf(f_err, "[ERRO] Linha %d | A Data de entrada é posterior à Data de saida. (Impossível)\n",
                    E.numE);
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
    int numTarifas = 0;
    Tarifa tarifas[MAX_TARIFAS];
    estacionamento E; // Variável temporária para guardar a linha atual
    printf("\n--- REGISTO DE ESTACIONAMENTOS ---\n");
    float precoPagar = CalcularPreco(E.diaE, E.mesE, E.anoE, E.horaE, E.minE, E.diaS, E.mesS, E.anoS, E.horaS, E.minS, tarifas, numTarifas);

    // O ciclo while funciona como um 'for' que corre enquanto houver linhas para ler
    // O fscanf retorna o número de variáveis lidas
    while (fscanf(f, "%d %s %d %d %d %d %d %s %d %d %d %d %d %f",
                  &E.numE,
                  E.matricula,
                  &E.anoE, &E.mesE, &E.diaE, &E.horaE, &E.minE, // Data Entrada
                  E.lugar,
                  &E.anoS, &E.mesS, &E.diaS, &E.horaS, &E.minS, // Data Saida
                  &precoPagar
                  ) == 14)
    {
        printf("Num: %-4d | Mat: %-8s | Lugar: %-4s | Ent: %02d/%02d/%d %02d:%02d | Sai: %02d/%02d/%d %02d:%02d | Preço: %.2f\n",
               E.numE,
               E.matricula,
               E.lugar,
               E.diaE, E.mesE, E.anoE, E.horaE, E.minE,
               E.diaS, E.mesS, E.anoS, E.horaS, E.minS,
               precoPagar
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
    float preco;
    while (fscanf(f_val, "%d %s %d %d %d %d %d %s %d %d %d %d %d %f",
                  &E.numE, E.matricula,
                  &E.anoE, &E.mesE, &E.diaE, &E.horaE, &E.minE,
                  E.lugar,
                  &E.anoS, &E.mesS, &E.diaS, &E.horaS, &E.minS,
                  &preco) == 14) {
        
        int entrouAntesOuIgual = ComparaDatas(E.diaE, E.mesE, E.anoE, E.horaE, E.minE, diaU, mesU, anoU, horaU, minU) <= 0;
        int saiuDepoisOuIgual  = ComparaDatas(E.diaS, E.mesS, E.anoS, E.horaS, E.minS, diaU, mesU, anoU, horaU, minU) >= 0;

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

// Função para inicializar o mapa (tudo livre)
void InicializarMapa(Lugar mapa[][MAX_FILAS][MAX_LUGARES], Confparque config) {
    for (int piso = 0; piso < config.numpisos; piso++) {
        for (int fila = 0; fila < config.numfilas; fila++) {
            for (int lugar = 0; lugar < config.numlugares; lugar++) {
                mapa[piso][fila][lugar].status = '-';
                strcpy(mapa[piso][fila][lugar].matricula, "");
            }
        }
    }
}

// Função para processar uma string de lugar (ex: "2A20")
void ProcessarLugar(char *lugarStr, int *piso, int *fila, int *numero) {
    // Piso: primeiro caractere (ex: '2' -> 2)
    *piso = lugarStr[0] - '0';
    
    // Fila: segundo caractere (ex: 'A' -> 0, 'B' -> 1, etc)
    *fila = lugarStr[1] - 'A';
    
    // Número: resto da string (ex: "20" -> 20)
    *numero = atoi(&lugarStr[2]);
}

// Função para ler o ficheiro de ocupação e marcar os lugares
void PreencherMapaComOcupacoes(Lugar mapa[][MAX_FILAS][MAX_LUGARES], char *ficheiroOcupacao) {
    FILE *f = fopen(ficheiroOcupacao, "r");
    
    if (f == NULL) {
        printf("Aviso: Ficheiro de ocupação não encontrado ou vazio.\n");
        return;
    }
    
    estacionamento E;
    int ocupados = 0;
    
    // Ler cada linha do ficheiro
    while (fscanf(f, "%d %s %d %d %d %d %d %s %d %d %d %d %d",
                  &E.numE, E.matricula,
                  &E.anoE, &E.mesE, &E.diaE, &E.horaE, &E.minE,
                  E.lugar,
                  &E.anoS, &E.mesS, &E.diaS, &E.horaS, &E.minS) == 13)
    {
        int piso, fila, numero;
        ProcessarLugar(E.lugar, &piso, &fila, &numero);
        
        // Converter para índices do array (começam em 0)
        piso--;    // Piso 1 -> índice 0
        numero--;  // Lugar 1 -> índice 0
        
        // Validar índices
        if (piso >= 0 && piso < MAX_PISOS &&
            fila >= 0 && fila < MAX_FILAS &&
            numero >= 0 && numero < MAX_LUGARES) {
            
            mapa[piso][fila][numero].status = 'X';
            strcpy(mapa[piso][fila][numero].matricula, E.matricula);
            ocupados++;
        }
    }
    
    fclose(f);
    printf("Total de lugares ocupados: %d\n\n", ocupados);
}

// Função para desenhar o mapa
void DesenharMapa(Lugar mapa[][MAX_FILAS][MAX_LUGARES], Confparque config) {
    
    for (int piso = 0; piso < config.numpisos; piso++) {
        printf("\n╔════════════════════════════════════════════════════════════╗\n");
        printf("║                      PISO %d                               ║\n", piso + 1);
        printf("╚════════════════════════════════════════════════════════════╝\n\n");
        
        // Cabeçalho com números dos lugares
        printf("    ");
        for (int lugar = 0; lugar < config.numlugares; lugar++) {
            printf("%2d ", lugar + 1);
        }
        printf("\n");
        
        printf("    ");
        for (int lugar = 0; lugar < config.numlugares; lugar++) {
            printf("───");
        }
        printf("\n");
        
        // Cada fila
        for (int fila = 0; fila < config.numfilas; fila++) {
            char letraFila = 'A' + fila;
            printf(" %c │ ", letraFila);
            
            for (int lugar = 0; lugar < config.numlugares; lugar++) {
                char status = mapa[piso][fila][lugar].status;
                
            }
            printf("\n");
        }
        printf("\n");
    }
    
    // Legenda
    printf("╔═══════════════════════════════════╗\n");
    printf("║            LEGENDA                ║\n");
    printf("╠═══════════════════════════════════╣\n");
    printf("║  \033[1;32m-\033[0m  = Lugar Livre             ║\n");
    printf("║  \033[1;31mX\033[0m  = Lugar Ocupado           ║\n");
    printf("╚═══════════════════════════════════╝\n");
}

// Função principal que junta tudo
void MostrarMapaOcupacao_ComMapa(Confparque config, char *ficheiroOcupacao,
                                 Lugar mapa[][MAX_FILAS][MAX_LUGARES]) {
    
    
    printf("\n MAPA DE OCUPAÇÃO DO PARQUE\n");
    printf("════════════════════════════════════════\n");
    
    // 1. Inicializar tudo como livre
    InicializarMapa(mapa, config);
    
    // 2. Ler ficheiro e marcar ocupados
    PreencherMapaComOcupacoes(mapa, ficheiroOcupacao);
    
    // 3. Desenhar o mapa
    DesenharMapa(mapa, config);
    
    // 4. Estatísticas
    int totalLugares = config.numpisos * config.numfilas * config.numlugares;
    int ocupados = 0;
    
    for (int p = 0; p < config.numpisos; p++) {
        for (int f = 0; f < config.numfilas; f++) {
            for (int l = 0; l < config.numlugares; l++) {
                if (mapa[p][f][l].status == 'X') {
                    ocupados++;
                }
            }
        }
    }
    
    float percentagem = (ocupados * 100.0) / totalLugares;
    
    printf("\n  ESTATÍSTICAS:\n");
    printf("   Total de lugares: %d\n", totalLugares);
    printf("   Lugares ocupados: %d\n", ocupados);
    printf("   Lugares livres: %d\n", totalLugares - ocupados);
    printf("   Taxa de ocupação: %.1f%%\n", percentagem);
}
