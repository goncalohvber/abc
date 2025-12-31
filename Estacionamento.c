//  Estacionamento.c
//  ProjetoEstacionamento
//
//  Created by Gonçalo Henrique Viegas Bernardino on 11/12/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include "Estacionamento.h"
#include "validacoes.h"
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
    
    // 🆕 ADICIONAR: Array para rastrear matrículas no parque
    char matriculasNoParque[MAX_REG_EST][10];
    int numMatriculasNoParque = 0;
    
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
        
        // 🆕 ADICIONAR: Verificar se matrícula já está no parque
        int matriculaDuplicada = 0;
        if (E.anoS == 0) {  // Se ainda não saiu
            for (int i = 0; i < numMatriculasNoParque; i++) {
                if (strcmp(matriculasNoParque[i], E.matricula) == 0) {
                    matriculaDuplicada = 1;
                    break;
                }
            }
        }
        
        if (entradaValida == 1 && saidaValida == 1 && matriculaValida == 1 &&
            lugarvalido == 1 && tempovalido == 1 && !matriculaDuplicada) {
            
            float precoPagar = CalcularPreco(E.diaE, E.mesE, E.anoE, E.horaE, E.minE,
                                           E.diaS, E.mesS, E.anoS, E.horaS, E.minS,
                                           tarifas, numTarifas);
            
            fprintf(f_val, "%d %s %d %d %d %d %d %s %d %d %d %d %d %.2f\n",
                    novoID, E.matricula,
                    E.anoE, E.mesE, E.diaE, E.horaE, E.minE,
                    E.lugar,
                    E.anoS, E.mesS, E.diaS, E.horaS, E.minS,
                    precoPagar);
            
            // 🆕 ADICIONAR: Se ainda não saiu, adicionar ao array
            if (E.anoS == 0 && numMatriculasNoParque < MAX_REG_EST) {
                strcpy(matriculasNoParque[numMatriculasNoParque], E.matricula);
                numMatriculasNoParque++;
            }
            
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
        // 🆕 ADICIONAR: Novo tipo de erro
        else if(matriculaDuplicada){
            fprintf(f_err, "[ERRO] Linha %d | Matricula: %s (Veículo já está no parque - entrada duplicada)\n",
                    E.numE, E.matricula);
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


// ============================================================
// FUNÇÃO AUXILIAR: Verificar se carro já está no parque
// ============================================================
int verificarCarroNoParque(char *matricula, char *ficheiroEstacionamentos) {
    FILE *f = fopen(ficheiroEstacionamentos, "r");
    if (f == NULL) {
        return 0; // Ficheiro não existe = nenhum carro no parque
    }
    
    estacionamento E;
    float preco;
    
    while (fscanf(f, "%d %s %d %d %d %d %d %s %d %d %d %d %d %f",
                  &E.numE, E.matricula,
                  &E.anoE, &E.mesE, &E.diaE, &E.horaE, &E.minE,
                  E.lugar,
                  &E.anoS, &E.mesS, &E.diaS, &E.horaS, &E.minS,
                  &preco) == 14)
    {
        // Se matrícula igual E ainda não saiu (ano saída = 0)
        if (strcmp(E.matricula, matricula) == 0 && E.anoS == 0) {
            fclose(f);
            return 1; // Carro já está no parque!
        }
    }
    
    fclose(f);
    return 0; // Carro não está no parque
}

// ============================================================
// FUNÇÃO AUXILIAR: Obter próximo número de entrada
// ============================================================
int obterProximoNumeroEntrada(char *ficheiroEstacionamentos) {
    FILE *f = fopen(ficheiroEstacionamentos, "r");
    if (f == NULL) {
        return 1;
    }
    
    int ultimoNum = 0;
    estacionamento E;
    
    // ✅ CORREÇÃO: Ler apenas 13 campos (sem preço)
    while (fscanf(f, "%d %s %d %d %d %d %d %s %d %d %d %d %d",
                  &E.numE, E.matricula,
                  &E.anoE, &E.mesE, &E.diaE, &E.horaE, &E.minE,
                  E.lugar,
                  &E.anoS, &E.mesS, &E.diaS, &E.horaS, &E.minS) == 13) {
        
        if (E.numE > ultimoNum) {
            ultimoNum = E.numE;
        }
    }
    
    fclose(f);
    return ultimoNum + 1;
}


int registarEntrada(Confparque config, char *ficheiroEstacionamentos) {
    estacionamento novoEstac;
    char matriculaTemp[10];
    int dia, mes, ano, hora, min;
    int carroJaNoParque = 0;
    
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║              ➕ REGISTAR ENTRADA DE VEÍCULO               ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n\n");
    
    // ========== PASSO 1: PEDIR DATA E HORA ==========
    do {
        printf("📅 Data de entrada (DD MM AAAA): ");
        scanf("%d %d %d", &dia, &mes, &ano);
        
        if (!validaData(dia, mes, ano)) {
            printf("❌ Data inválida! Tente novamente.\n\n");
        }
    } while (!validaData(dia, mes, ano));
    
    do {
        printf("🕐 Hora de entrada (HH MM): ");
        scanf("%d %d", &hora, &min);
        
        if (hora < 0 || hora > 23 || min < 0 || min > 59) {
            printf("❌ Hora inválida! Tente novamente.\n\n");
        }
    } while (hora < 0 || hora > 23 || min < 0 || min > 59);
    
    // ========== PASSO 2: PEDIR MATRÍCULA ==========
    do {
        printf("🚗 Matrícula do veículo (XX-XX-XX): ");
        scanf("%s", matriculaTemp);
        
        if (!validamatricula(matriculaTemp)) {
            printf("❌ Matrícula inválida! Formato correto: XX-XX-XX\n\n");
            continue;
        }
        
        if (verificarCarroNoParque(matriculaTemp, "estacionamentos.txt")) {
            printf("❌ ERRO: O veículo %s já se encontra no parque!\n", matriculaTemp);
            printf("   Por favor, verifique a matrícula ou registe a saída primeiro.\n\n");
            carroJaNoParque = 1;
        } else {
            carroJaNoParque = 0;
        }
        
    } while (!validamatricula(matriculaTemp) || carroJaNoParque);
    
    // ========== PASSO 3: ATRIBUIR LUGAR ==========
    char *lugarAtribuido = atribuirLugar(config, "estacionamentos.txt");
    
    if (lugarAtribuido == NULL) {
        printf("\n❌ ERRO: Não há lugares disponíveis no parque!\n");
        return 0;
    }
    
    // ========== PASSO 4: PREENCHER ESTRUTURA ==========
    // 🆕 Obter AMBOS os números (base e validado)
    novoEstac.numE = obterProximoNumeroEntrada("estacionamentos.txt");
    novoEstac.numValidado = obterProximoNumeroEntrada("estacionamentos_validos.txt");
    
    strcpy(novoEstac.matricula, matriculaTemp);
    novoEstac.anoE = ano;
    novoEstac.mesE = mes;
    novoEstac.diaE = dia;
    novoEstac.horaE = hora;
    novoEstac.minE = min;
    strcpy(novoEstac.lugar, lugarAtribuido);
    
    // Data de saída = 0 (ainda não saiu)
    novoEstac.anoS = 0;
    novoEstac.mesS = 0;
    novoEstac.diaS = 0;
    novoEstac.horaS = 0;
    novoEstac.minS = 0;
    
    // ========== PASSO 5: GRAVAR NO FICHEIRO BASE ==========
    FILE *f = fopen("estacionamentos.txt", "a");
    
    if (f == NULL) {
        printf("❌ ERRO: Não foi possível abrir o ficheiro!\n");
        return 0;
    }
    
    fprintf(f, "%d\t%s\t%d\t%d\t%d\t%d\t%d\t%s\t%d\t%d\t%d\t%d\t%d\n",
            novoEstac.numE,
            novoEstac.matricula,
            novoEstac.anoE, novoEstac.mesE, novoEstac.diaE,
            novoEstac.horaE, novoEstac.minE,
            novoEstac.lugar,
            novoEstac.anoS, novoEstac.mesS, novoEstac.diaS,
            novoEstac.horaS, novoEstac.minS);
    
    fclose(f);
    
    // ========== 🆕 PASSO 6: GRAVAR NO FICHEIRO VALIDADO ==========
    f = fopen("estacionamentos_validos.txt", "a");
    
    if (f == NULL) {
        printf("❌ ERRO: Não foi possível abrir o ficheiro validado!\n");
        return 0;
    }
    
    // Gravar com o número validado e preço 0.00 (ainda não saiu)
    fprintf(f, "%d %s %d %d %d %d %d %s %d %d %d %d %d %.2f\n",
            novoEstac.numValidado,  // 🆕 Usar número validado
            novoEstac.matricula,
            novoEstac.anoE, novoEstac.mesE, novoEstac.diaE,
            novoEstac.horaE, novoEstac.minE,
            novoEstac.lugar,
            novoEstac.anoS, novoEstac.mesS, novoEstac.diaS,
            novoEstac.horaS, novoEstac.minS,
            0.00);  // Preço zero porque ainda não saiu
    
    fclose(f);
    
    // ========== PASSO 7: MOSTRAR TICKET ==========
    printf("\n✅ Entrada registada com sucesso!\n");
    mostrarTicket(novoEstac);
    
    return 1;
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

// ============================================================
// FUNÇÃO AUXILIAR: Atribuir lugar automático
// ============================================================
char* atribuirLugar(Confparque config, char *ficheiroEstacionamentos) {
    // Criar array para marcar lugares ocupados
    static char lugarAtribuido[10];
    int ocupados[MAX_PISOS][MAX_FILAS][MAX_LUGARES] = {0};
    
    // Ler ficheiro e marcar lugares ocupados
    FILE *f = fopen(ficheiroEstacionamentos, "r");
    if (f != NULL) {
        estacionamento E;
        float preco;
        
        while (fscanf(f, "%d %s %d %d %d %d %d %s %d %d %d %d %d %f",
                      &E.numE, E.matricula,
                      &E.anoE, &E.mesE, &E.diaE, &E.horaE, &E.minE,
                      E.lugar,
                      &E.anoS, &E.mesS, &E.diaS, &E.horaS, &E.minS,
                      &preco) == 14)
        {
            // Se ainda não saiu (anoS = 0), marcar como ocupado
            if (E.anoS == 0) {
                int piso, fila, numero;
                ProcessarLugar(E.lugar, &piso, &fila, &numero);
                ocupados[piso-1][fila][numero-1] = 1;
            }
        }
        fclose(f);
    }
    
    // Procurar primeiro lugar livre (ordem: piso -> fila -> número)
    for (int p = 0; p < config.numpisos; p++) {
        for (int f = 0; f < config.numfilas; f++) {
            for (int l = 0; l < config.numlugares; l++) {
                if (ocupados[p][f][l] == 0) {
                    // Encontrou lugar livre!
                    sprintf(lugarAtribuido, "%d%c%d",
                            p+1,           // Piso (1-5)
                            'A' + f,       // Fila (A-Z)
                            l+1);          // Número (1-50)
                    return lugarAtribuido;
                }
            }
        }
    }
    
    // Sem lugares livres
    return NULL;
}

void mostrarTicket(estacionamento E) {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║              🎫 TICKET DE ESTACIONAMENTO                  ║\n");
    printf("╠═══════════════════════════════════════════════════════════╣\n");
    printf("║                                                            ║\n");
    printf("║  Nº Entrada: %-6d                                        ║\n", E.numValidado);  
    printf("║  Matrícula:  %-10s                                      ║\n", E.matricula);
    printf("║                                                            ║\n");
    printf("║  📍 Lugar Atribuído: %-5s                                ║\n", E.lugar);
    printf("║                                                            ║\n");
    printf("║  📅 Data Entrada: %02d/%02d/%d                              ║\n",
           E.diaE, E.mesE, E.anoE);
    printf("║  🕐 Hora Entrada: %02d:%02d                                   ║\n",
           E.horaE, E.minE);
    printf("║                                                            ║\n");
    printf("║  ⚠️  Guarde este ticket para efetuar o pagamento!         ║\n");
    printf("║                                                            ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
}

// ============================================================
// FUNÇÃO PRINCIPAL: Registar Saída
// ============================================================
int registarSaida(Confparque config, char *ficheiroEstacionamentos) {
    char matriculaProcurada[10];
    int dia, mes, ano, hora, min;
    int encontrado = 0;
    
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║              ➖ REGISTAR SAÍDA DE VEÍCULO                ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n\n");
    
    // ========== PASSO 1: PEDIR MATRÍCULA ==========
    do {
        printf("🚗 Matrícula do veículo (XX-XX-XX): ");
        scanf("%s", matriculaProcurada);
        
        if (!validamatricula(matriculaProcurada)) {
            printf("❌ Matrícula inválida! Formato correto: XX-XX-XX\n\n");
        }
    } while (!validamatricula(matriculaProcurada));
    
    // ========== PASSO 2: VERIFICAR SE ESTÁ NO PARQUE ==========
    // 🆕 MODIFICAÇÃO: Ler de estacionamentos.txt
    FILE *f = fopen("estacionamentos.txt", "r");
    if (f == NULL) {
        printf("❌ ERRO: Não foi possível abrir o ficheiro!\n");
        return 0;
    }
    
    estacionamento E;
    
    // 🆕 Procurar o veículo (sem ler preço, porque ainda não foi calculado)
    while (fscanf(f, "%d %s %d %d %d %d %d %s %d %d %d %d %d",
                  &E.numE, E.matricula,
                  &E.anoE, &E.mesE, &E.diaE, &E.horaE, &E.minE,
                  E.lugar,
                  &E.anoS, &E.mesS, &E.diaS, &E.horaS, &E.minS) == 13)
    {
        if (strcmp(E.matricula, matriculaProcurada) == 0 && E.anoS == 0) {
            encontrado = 1;
            break;
        }
    }
    fclose(f);
    
    if (!encontrado) {
        printf("\n❌ ERRO: O veículo %s não se encontra no parque!\n", matriculaProcurada);
        printf("   Verifique se a matrícula está correta.\n");
        return 0;
    }
    
    // ========== PASSO 3: MOSTRAR INFO DA ENTRADA ==========
    printf("\n✅ Veículo encontrado no parque!\n");
    printf("┌─────────────────────────────────────────────┐\n");
    printf("│ INFORMAÇÃO DO ESTACIONAMENTO                │\n");
    printf("├─────────────────────────────────────────────┤\n");
    printf("│ Nº Entrada: %-6d                         │\n", E.numE);
    printf("│ Matrícula:  %-10s                       │\n", E.matricula);
    printf("│ Lugar:      %-5s                          │\n", E.lugar);
    printf("│ Entrada:    %02d/%02d/%d às %02d:%02d           │\n",
           E.diaE, E.mesE, E.anoE, E.horaE, E.minE);
    printf("└─────────────────────────────────────────────┘\n\n");
    
    // ========== PASSO 4: PEDIR DATA E HORA DE SAÍDA ==========
    do {
        printf("📅 Data de saída (DD MM AAAA): ");
        scanf("%d %d %d", &dia, &mes, &ano);
        
        if (!validaData(dia, mes, ano)) {
            printf("❌ Data inválida! Tente novamente.\n\n");
        }
    } while (!validaData(dia, mes, ano));
    
    do {
        printf("🕐 Hora de saída (HH MM): ");
        scanf("%d %d", &hora, &min);
        
        if (hora < 0 || hora > 23 || min < 0 || min > 59) {
            printf("❌ Hora inválida! Tente novamente.\n\n");
        }
    } while (hora < 0 || hora > 23 || min < 0 || min > 59);
    
    // ========== PASSO 5: VALIDAR QUE SAÍDA É DEPOIS DA ENTRADA ==========
    if (!validaEantesS(E.diaE, E.mesE, E.anoE, E.horaE, E.minE,
                       dia, mes, ano, hora, min)) {
        printf("\n❌ ERRO: A data/hora de saída deve ser posterior à entrada!\n");
        printf("   Entrada: %02d/%02d/%d às %02d:%02d\n", E.diaE, E.mesE, E.anoE, E.horaE, E.minE);
        printf("   Saída:   %02d/%02d/%d às %02d:%02d\n", dia, mes, ano, hora, min);
        return 0;
    }
    
    // ========== PASSO 6: CARREGAR TARIFAS (NÃO CALCULAR PREÇO) ==========
    // 🆕 NÃO calculamos preço aqui, será calculado na validação
    
    // ========== PASSO 7: ATUALIZAR O FICHEIRO ==========
    // 🆕 MODIFICAÇÃO: Atualizar estacionamentos.txt
    FILE *f_temp = fopen("temp_estacionamentos.txt", "w");
    if (f_temp == NULL) {
        printf("❌ ERRO: Não foi possível criar ficheiro temporário!\n");
        return 0;
    }
    
    f = fopen("estacionamentos.txt", "r");
    if (f == NULL) {
        printf("❌ ERRO: Não foi possível reabrir o ficheiro!\n");
        fclose(f_temp);
        return 0;
    }
    
    // Copiar todos os registos, atualizando o correto
    while (fscanf(f, "%d %s %d %d %d %d %d %s %d %d %d %d %d",
                  &E.numE, E.matricula,
                  &E.anoE, &E.mesE, &E.diaE, &E.horaE, &E.minE,
                  E.lugar,
                  &E.anoS, &E.mesS, &E.diaS, &E.horaS, &E.minS) == 13)
    {
        if (strcmp(E.matricula, matriculaProcurada) == 0 && E.anoS == 0) {
            // Este é o registo a atualizar (com TABS)
            fprintf(f_temp, "%d\t%s\t%d\t%d\t%d\t%d\t%d\t%s\t%d\t%d\t%d\t%d\t%d\n",
                    E.numE, E.matricula,
                    E.anoE, E.mesE, E.diaE, E.horaE, E.minE,
                    E.lugar,
                    ano, mes, dia, hora, min);  // ← Nova data/hora de saída
        } else {
            // Copiar o registo sem alterações (com TABS)
            fprintf(f_temp, "%d\t%s\t%d\t%d\t%d\t%d\t%d\t%s\t%d\t%d\t%d\t%d\t%d\n",
                    E.numE, E.matricula,
                    E.anoE, E.mesE, E.diaE, E.horaE, E.minE,
                    E.lugar,
                    E.anoS, E.mesS, E.diaS, E.horaS, E.minS);
        }
    }
    
    fclose(f);
    fclose(f_temp);
    
    // Substituir o ficheiro original pelo temporário
    remove("estacionamentos.txt");
    rename("temp_estacionamentos.txt", "estacionamentos.txt");
    
    // ========== PASSO 8: CALCULAR PREÇO PARA MOSTRAR ==========
    Tarifa tarifas[MAX_TARIFAS];
    int numTarifas = 0;
    
    if (!lertarifas(tarifas, &numTarifas)) {
        printf("❌ ERRO: Não foi possível carregar as tarifas!\n");
        return 0;
    }
    
    float precoPagar = CalcularPreco(E.diaE, E.mesE, E.anoE, E.horaE, E.minE,
                                     dia, mes, ano, hora, min,
                                     tarifas, numTarifas);
    
    // ========== PASSO 9: MOSTRAR RECIBO ==========
    printf("\n✅ Saída registada com sucesso!\n");
    mostrarRecibo(E.numE, matriculaProcurada, E.lugar,
                  E.diaE, E.mesE, E.anoE, E.horaE, E.minE,
                  dia, mes, ano, hora, min,
                  precoPagar);
    
    return 1;
}


// ============================================================
// FUNÇÃO AUXILIAR: Mostrar recibo de saída
// ============================================================
void mostrarRecibo(int numE, char *matricula, char *lugar,
                   int diaE, int mesE, int anoE, int horaE, int minE,
                   int diaS, int mesS, int anoS, int horaS, int minS,
                   float preco) {
    
    // Calcular duração
    int totalMinutos = 0;
    
    // Converter datas para minutos totais (simplificado)
    int minutosEntrada = (diaE * 24 * 60) + (horaE * 60) + minE;
    int minutosSaida = (diaS * 24 * 60) + (horaS * 60) + minS;
    
    // Ajustar para mudança de mês/ano (aproximação)
    if (anoS > anoE) {
        minutosSaida += (anoS - anoE) * 365 * 24 * 60;
    }
    if (mesS > mesE) {
        minutosSaida += (mesS - mesE) * 30 * 24 * 60;
    }
    
    totalMinutos = minutosSaida - minutosEntrada;
    
    int dias = totalMinutos / (24 * 60);
    int horas = (totalMinutos % (24 * 60)) / 60;
    int minutos = totalMinutos % 60;
    
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║              🧾 RECIBO DE ESTACIONAMENTO                  ║\n");
    printf("╠═══════════════════════════════════════════════════════════╣\n");
    printf("║                                                           ║\n");
    printf("║  Nº Entrada: %-6d                                       ║\n", numE);
    printf("║  Matrícula:  %-10s                                     ║\n", matricula);
    printf("║  Lugar:      %-5s                                        ║\n", lugar);
    printf("║                                                           ║\n");
    printf("║  ───────────────────────────────────────────────────────  ║\n");
    printf("║                                                           ║\n");
    printf("║  📅 Entrada: %02d/%02d/%d às %02d:%02d                       ║\n",
           diaE, mesE, anoE, horaE, minE);
    printf("║  📅 Saída:   %02d/%02d/%d às %02d:%02d                       ║\n",
           diaS, mesS, anoS, horaS, minS);
    printf("║                                                           ║\n");
    printf("║  ⏱️  Duração: ");
    if (dias > 0) printf("%d dia(s), ", dias);
    printf("%02d:%02d:%02d", horas, minutos / 60, minutos % 60);
    // Preencher espaços para alinhar
    int espacos = 28 - (dias > 0 ? 10 : 0);
    for (int i = 0; i < espacos; i++) printf(" ");
    printf("║\n");
    printf("║                                                           ║\n");
    printf("║  ───────────────────────────────────────────────────────  ║\n");
    printf("║                                                           ║\n");
    printf("║  💰 VALOR A PAGAR: %.2f €                              ║\n", preco);
    printf("║                                                           ║\n");
    printf("║  Obrigado pela preferência!                               ║\n");
    printf("║                                                           ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
}

// ============================================================
// FUNÇÃO AUXILIAR: Obter próximo número de entrada VALIDADO
// ============================================================
int obterProximoNumeroValidado(char *ficheiroValidado) {
    FILE *f = fopen(ficheiroValidado, "r");
    if (f == NULL) {
        return 1; // Primeiro registo
    }
    
    int ultimoNumValidado = 0;  // 🆕 Nome mais claro
    int numLido;                 // 🆕 Variável temporária para ler o número
    char matricula[MAX_MATRICULA];
    int anoE, mesE, diaE, horaE, minE;
    char lugar[MAX_LUGAR];
    int anoS, mesS, diaS, horaS, minS;
    float preco;
    
    while (fscanf(f, "%d %s %d %d %d %d %d %s %d %d %d %d %d %f",
                  &numLido, matricula,           // 🆕 Lê o número validado
                  &anoE, &mesE, &diaE, &horaE, &minE,
                  lugar,
                  &anoS, &mesS, &diaS, &horaS, &minS,
                  &preco) == 14)
    {
        if (numLido > ultimoNumValidado) {      // 🆕 Compara com o número lido
            ultimoNumValidado = numLido;
        }
    }
    
    fclose(f);
    return ultimoNumValidado + 1;
}

