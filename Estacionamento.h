#ifndef Estacionamento_h
#define Estacionamento_h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Instalacao.h"
#include "Tarifas.h"
#define MAX_MATRICULA 15
#define MAX_LUGAR 5
#define FICHEIRO_ESTAC_BIN "estacionamentos.bin"
#define FICHEIRO_OCUPACAO_BIN "ocupacao.bin"
#define FICHEIRO_CONTADOR_BIN "contador.bin"

typedef struct {
    int numE;                    
    char matricula[MAX_MATRICULA];
    int anoE;                    
    int mesE;                    
    int diaE;                   
    int horaE;                   
    int minE;                    
    char lugar[MAX_LUGAR];       
    int anoS;                    
    int mesS;                    
    int diaS;                    
    int horaS;                   
    int minS;                    
    float valorPago;            
    int ativo;                   
} estacionamento;

typedef struct {
    char status;                 
    char matricula[MAX_MATRICULA]; 
    char motivo;                 
} Lugar;

typedef struct {
    int ultimoNumE;              
    int totalEstacionamentos;    
} ContadorSistema;

int carregarEstacionamentosTXT(char *nomeficheiro, estacionamento est[],
                               int *numEst, Confparque config);

int guardarEstacionamentosBIN(estacionamento est[], int numEst);

int carregarEstacionamentosBIN(estacionamento est[], int *numEst);

int guardarOcupacaoBIN(Lugar mapa[][MAX_FILAS][MAX_LUGARES], Confparque config);

int carregarOcupacaoBIN(Lugar mapa[][MAX_FILAS][MAX_LUGARES], Confparque config);

int guardarContadorBIN(ContadorSistema *contador);

int carregarContadorBIN(ContadorSistema *contador);

int guardarTodosSistema(estacionamento est[], int numEst,
                        Lugar mapa[][MAX_FILAS][MAX_LUGARES],
                        Confparque config, ContadorSistema *contador);

int carregarTodosSistema(estacionamento est[], int *numEst,
                         Lugar mapa[][MAX_FILAS][MAX_LUGARES],
                         Confparque config, ContadorSistema *contador);

int validaData(int dia, int mes, int ano);
int validamatricula(char *mat);
int validaLugar(char *lugar, int maxPisos, int maxFila, int maxLugares);
int validaEantesS(int diaE, int mesE, int anoE, int horaE, int minE,
                  int diaS, int mesS, int anoS, int horaS, int minS);
int ComparaDatas(int d1, int m1, int a1, int h1, int min1,
                 int d2, int m2, int a2, int h2, int min2);

void LimpaFicheiroEstac(char *ficheirobase, char *ficheirovalido,
                        char *ficheiroerros, Confparque config);
void MostrarFicheiroEstacionamento(const char *nomeFicheiro);
void gerarficheiroocupacao(char *ficheirovalido, char *ficheiroocupacao,
                           int diaU, int mesU, int anoU, int horaU, int minU);

void InicializarMapa(Lugar mapa[][MAX_FILAS][MAX_LUGARES], Confparque config);
void ProcessarLugar(char *lugarStr, int *piso, int *fila, int *numero);
void PreencherMapaComOcupacoes(Lugar mapa[][MAX_FILAS][MAX_LUGARES],
                               char *ficheiroOcupacao);
void DesenharMapa(Lugar mapa[][MAX_FILAS][MAX_LUGARES], Confparque config);
void MostrarMapaOcupacao_ComMapa(Confparque config, char *ficheiroOcupacao,
                                 Lugar mapa[][MAX_FILAS][MAX_LUGARES]);

float CalcularPreco(int dE, int mE, int aE, int hE, int minE,
                    int dS, int mS, int aS, int hS, int minS,
                    Tarifa tarifas[], int numTarifas);

int criarBackup(void);

#endif /* Estacionamento_h */
