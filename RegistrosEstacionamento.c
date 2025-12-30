#include "Estacionamento.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int verificarCarroNoParque(char *matricula, char *ficheiroEstacionamentos) {
    FILE *f = fopen(ficheiroEstacionamentos, "r");
    if (f == NULL) {
        return 0;
    }

    char mat[MAX_MATRICULA];
    int numE, anoE, mesE, diaE, horaE, minE;
    int anoS, mesS, diaS, horaS, minS;
    char lugar[MAX_LUGAR];

    while (fscanf(f, "%d %s %d %d %d %d %d %s %d %d %d %d %d",
                  &numE, mat,
                  &anoE, &mesE, &diaE, &horaE, &minE,
                  lugar,
                  &anoS, &mesS, &diaS, &horaS, &minS) == 13) {

        if (strcmp(mat, matricula) == 0 && anoS == 0) {
            fclose(f);
            return 1; 
        }
    }

    fclose(f);
    return 0; 
}

int obterProximoNumeroEntrada(char *ficheiroEstacionamentos) {
    FILE *f = fopen(ficheiroEstacionamentos, "r");

    if (f == NULL) {
        return 1;
    }

    int ultimoNumero = 0;
    int numE;
    char linha[500];

    while (fgets(linha, sizeof(linha), f) != NULL) {
        if (sscanf(linha, "%d", &numE) == 1) {
            if (numE > ultimoNumero) {
                ultimoNumero = numE;
            }
        }
    }
    fclose(f);
    return ultimoNumero + 1; 
}

char* atribuirLugar(Confparque config, char *ficheiroEstacionamentos) {
    static char lugarAtribuido[MAX_LUGAR];

    int ocupados[MAX_PISOS][MAX_FILAS][MAX_LUGARES];

    for (int p = 0; p < config.numpisos; p++) {
        for (int f = 0; f < config.numfilas; f++) {
            for (int l = 0; l < config.numlugares; l++) {
                ocupados[p][f][l] = 0;
            }
        }
    }
    FILE *fich = fopen(ficheiroEstacionamentos, "r");
    if (fich != NULL) {
        char lugar[MAX_LUGAR];
        int numE, anoE, mesE, diaE, horaE, minE;
        int anoS, mesS, diaS, horaS, minS;
        char matricula[MAX_MATRICULA];

        while (fscanf(fich, "%d %s %d %d %d %d %d %s %d %d %d %d %d",
                      &numE, matricula,
                      &anoE, &mesE, &diaE, &horaE, &minE,
                      lugar,
                      &anoS, &mesS, &diaS, &horaS, &minS) == 13) {
            if (anoS == 0) {
                int piso = lugar[0] - '0' - 1;  // Converter para índice
                int fila = lugar[1] - 'A';      // Converter para índice
                int numero = atoi(&lugar[2]) - 1; // Converter para índice

                if (piso >= 0 && piso < config.numpisos &&
                    fila >= 0 && fila < config.numfilas &&
                    numero >= 0 && numero < config.numlugares) {

                    ocupados[piso][fila][numero] = 1;
                }
            }
        }
        fclose(fich);
    }
    for (int p = 0; p < config.numpisos; p++) {
        for (int f = 0; f < config.numfilas; f++) {
            for (int l = 0; l < config.numlugares; l++) {
                if (ocupados[p][f][l] == 0) {
                    sprintf(lugarAtribuido, "%d%c%02d",
                            p + 1,           // Piso (1-5)
                            'A' + f,         // Fila (A-Z)
                            l + 1);          // Número (1-50)
                    return lugarAtribuido;
                }
            }
        }
    }
    return NULL;
}

void atualizarValidacao(Confparque config) {
    printf("\n Atualizando ficheiro validado...\n");

    // Revalidar o ficheiro estacionamentos.txt
    LimpaFicheiroEstac("estacionamentos.txt",
                       "estacionamentos_validos.txt",
                       "relatorio_erros.txt",
                       config);

    printf(" Ficheiro atualizado!\n");
}
