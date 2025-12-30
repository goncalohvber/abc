#include "Estacionamento.h"
#include "Tarifas.h"

int carregarEstacionamentosTXT(char *nomeficheiro, estacionamento est[],
                               int *numEst, Confparque config) {

    FILE *f = fopen(nomeficheiro, "r");
    if (f == NULL) {
        printf("AVISO: Ficheiro %s não encontrado.\n", nomeficheiro);
        *numEst = 0;
        return 0;
    }
    printf("\n Carregando estacionamentos de %s...\n", nomeficheiro);
    *numEst = 0;
    float preco;
    while (fscanf(f, "%d %s %d %d %d %d %d %s %d %d %d %d %d %f",
                  &est[*numEst].numE,
                  est[*numEst].matricula,
                  &est[*numEst].anoE, &est[*numEst].mesE, &est[*numEst].diaE,
                  &est[*numEst].horaE, &est[*numEst].minE,
                  est[*numEst].lugar,
                  &est[*numEst].anoS, &est[*numEst].mesS, &est[*numEst].diaS,
                  &est[*numEst].horaS, &est[*numEst].minS,
                  &preco) == 14) {
        est[*numEst].valorPago = preco;
        est[*numEst].ativo = 1;
        (*numEst)++;
        if (*numEst >= MAX_REG_EST) {
            printf("  ATENÇÃO: Limite de %d registos atingido!\n", MAX_REG_EST);
            break;
        }
    }

    fclose(f);

    printf(" %d estacionamentos carregados com sucesso!\n\n", *numEst);
    return 1;
}

int guardarEstacionamentosBIN(estacionamento est[], int numEst) {

    FILE *f = fopen(FICHEIRO_ESTAC_BIN, "wb");
    if (f == NULL) {
        printf(" ERRO: Não foi possível criar %s\n", FICHEIRO_ESTAC_BIN);
        return 0;
    }
    fwrite(&numEst, sizeof(int), 1, f);
    fwrite(est, sizeof(estacionamento), numEst, f);
    fclose(f);
    printf(" %d estacionamentos guardados em %s\n", numEst, FICHEIRO_ESTAC_BIN);
    return 1;
}

int carregarEstacionamentosBIN(estacionamento est[], int *numEst) {

    FILE *f = fopen(FICHEIRO_ESTAC_BIN, "rb");
    if (f == NULL) {
        printf("  Ficheiro %s não existe. Primeira execução.\n", FICHEIRO_ESTAC_BIN);
        *numEst = 0;
        return 0;
    }
    fread(numEst, sizeof(int), 1, f);
    if (*numEst < 0 || *numEst > MAX_REG_EST) {
        printf(" ERRO: Número de registos inválido (%d)\n", *numEst);
        fclose(f);
        *numEst = 0;
        return 0;
    }
    fread(est, sizeof(estacionamento), *numEst, f);
    fclose(f);
    printf(" %d estacionamentos carregados de %s\n", *numEst, FICHEIRO_ESTAC_BIN);
    return 1;
}

int guardarOcupacaoBIN(Lugar mapa[][MAX_FILAS][MAX_LUGARES], Confparque config) {

    FILE *f = fopen(FICHEIRO_OCUPACAO_BIN, "wb");
    if (f == NULL) {
        printf(" ERRO: Não foi possível criar %s\n", FICHEIRO_OCUPACAO_BIN);
        return 0;
    }
    fwrite(&config, sizeof(Confparque), 1, f);
    for (int p = 0; p < config.numpisos; p++) {
        for (int fil = 0; fil < config.numfilas; fil++) {  // <-- fil aqui
            fwrite(mapa[p][fil], sizeof(Lugar), config.numlugares, f);  // <-- e aqui
        }
    }

    fclose(f);
    printf(" Mapa de ocupação guardado\n");
    return 1;
}

int carregarOcupacaoBIN(Lugar mapa[][MAX_FILAS][MAX_LUGARES], Confparque config) {

    FILE *f = fopen(FICHEIRO_OCUPACAO_BIN, "rb");
    if (f == NULL) {
        printf("  Ficheiro %s não existe. Mapa inicializado vazio.\n",
               FICHEIRO_OCUPACAO_BIN);
        InicializarMapa(mapa, config);
        return 0;
    }
    Confparque configGuardado;
    fread(&configGuardado, sizeof(Confparque), 1, f);

    
    if (configGuardado.numpisos != config.numpisos ||
        configGuardado.numfilas != config.numfilas ||
        configGuardado.numlugares != config.numlugares) {

        printf("️ AVISO: Configuração mudou! A inicializar mapa vazio.\n");
        fclose(f);
        InicializarMapa(mapa, config);
        return 0;
    }

    for (int p = 0; p < config.numpisos; p++) {
        for (int fil = 0; fil < config.numfilas; fil++) {  // <-- fil aqui
            fread(mapa[p][fil], sizeof(Lugar), config.numlugares, f);  // <-- e aqui
        }
    }

    fclose(f);
    printf(" Mapa de ocupação carregado\n");
    return 1;
}

int guardarContadorBIN(ContadorSistema *contador) {

    FILE *f = fopen(FICHEIRO_CONTADOR_BIN, "wb");
    if (f == NULL) {
        printf(" ERRO: Não foi possível criar %s\n", FICHEIRO_CONTADOR_BIN);
        return 0;
    }

    fwrite(contador, sizeof(ContadorSistema), 1, f);
    fclose(f);

    printf(" Contador guardado (último nº: %d)\n", contador->ultimoNumE);
    return 1;
}

int carregarContadorBIN(ContadorSistema *contador) {

    FILE *f = fopen(FICHEIRO_CONTADOR_BIN, "rb");
    if (f == NULL) {
        printf("  Ficheiro %s não existe. Contador inicializado.\n",
               FICHEIRO_CONTADOR_BIN);
        contador->ultimoNumE = 0;
        contador->totalEstacionamentos = 0;
        return 0;
    }

    fread(contador, sizeof(ContadorSistema), 1, f);
    fclose(f);

    printf(" Contador carregado (último nº: %d)\n", contador->ultimoNumE);
    return 1;
}

int guardarTodosSistema(estacionamento est[], int numEst,
                        Lugar mapa[][MAX_FILAS][MAX_LUGARES],
                        Confparque config, ContadorSistema *contador) {

    printf("\n════════════════════════════════════\n");
    printf("          GUARDANDO DADOS...        \n");
    printf("════════════════════════════════════\n");

    int ok1 = guardarEstacionamentosBIN(est, numEst);
    int ok2 = guardarOcupacaoBIN(mapa, config);
    int ok3 = guardarContadorBIN(contador);

    if (ok1 && ok2 && ok3) {
        printf("════════════════════════════════════\n");
        printf("     TODOS OS DADOS GUARDADOS!    \n");
        printf("════════════════════════════════════\n\n");
        return 1;
    } else {
        printf("════════════════════════════════════\n");
        printf("     ALGUNS DADOS NÃO GUARDARAM!    \n");
        printf("════════════════════════════════════\n\n");
        return 0;
    }
}

int carregarTodosSistema(estacionamento est[], int *numEst,
                         Lugar mapa[][MAX_FILAS][MAX_LUGARES],
                         Confparque config, ContadorSistema *contador) {

    printf("\n════════════════════════════════════\n");
    printf("          CARREGANDO DADOS...       \n");
    printf("════════════════════════════════════\n");

    int ok1 = carregarEstacionamentosBIN(est, numEst);
    int ok2 = carregarOcupacaoBIN(mapa, config);
    int ok3 = carregarContadorBIN(contador);


    if (!ok1) {
        printf("\n  Tentando carregar de ficheiro texto...\n");
        carregarEstacionamentosTXT("estacionamentos_validos.txt",
                                   est, numEst, config);
    }

    if (!ok2) {
        InicializarMapa(mapa, config);
    }

    printf("════════════════════════════════════\n");
    printf("          SISTEMA PRONTO            \n");
    printf("════════════════════════════════════\n\n");

    return 1;
}
