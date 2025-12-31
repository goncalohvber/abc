//
//  Persistencia.h
//  ProjetoEstacionamento
//
//  Created by Gonçalo Henrique Viegas Bernardino on 31/12/2025.
//

#ifndef Persistencia_h
#define Persistencia_h

#include "Estacionamento.h"
#include "Instalacao.h"

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

#endif /* Persistencia_h */
