#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// =========================================================
// CADASTRAR TERRITÓRIOS
// =========================================================
void cadastrarTerritorios(struct Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\n--- Cadastro do Territorio %d ---\n", i + 1);

        printf("Nome: ");
        fgets(mapa[i].nome, 30, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0;

        printf("Cor (dono): ");
        fgets(mapa[i].cor, 10, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = 0;

        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
        getchar();
    }
}

// =========================================================
// EXIBIR MAPA
// =========================================================
void exibirMapa(struct Territorio* mapa, int n) {
    printf("\n===== ESTADO ATUAL DO MAPA =====\n");
    for (int i = 0; i < n; i++) {
        printf("\nTerritorio %d\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor: %s\n", mapa[i].cor);
        printf("Tropas: %d\n", mapa[i].tropas);
    }
    printf("=================================\n");
}

// =========================================================
// FUNÇÃO DE ATAQUE
// =========================================================
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    printf("\n--- BATALHA DESTE TURNO ---\n");

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("%s (ATACANTE) rolou: %d\n", atacante->nome, dadoAtacante);
    printf("%s (DEFENSOR) rolou: %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        defensor->tropas -= 1;
        printf("Defensor perdeu 1 tropa! Restam %d\n", defensor->tropas);
    }
    else if (dadoDefensor > dadoAtacante) {
        atacante->tropas -= 1;
        printf("Atacante perdeu 1 tropa! Restam %d\n", atacante->tropas);
    }
    else {
        printf("Empate! Ninguem perdeu tropas.\n");
    }

    // Conquista
    if (defensor->tropas <= 0) {
        printf("\n=== TERRITORIO CONQUISTADO! ===\n");
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        printf("%s agora pertence a cor %s\n", defensor->nome, defensor->cor);
        printf("Tropas movidas: %d\n", defensor->tropas);
    }

    printf("\n--- FIM DA BATALHA DO TURNO ---\n");
}

// =========================================================
// ATRIBUIR MISSÃO (ALEATÓRIA E SEM REPETIÇÃO)
// =========================================================
void atribuirMissao(char* destino, char* origem) {
    strcpy(destino, origem);
}

// =========================================================
// VERIFICAR MISSÃO
// (REGRAS SIMPLIFICADAS MAS FUNCIONAIS)
// =========================================================
int verificarMissao(char* missao, struct Territorio* mapa, int n, char* corJogador) {

    // 1. "Conquistar pelo menos 2 territórios"
    if (strcmp(missao, "Conquistar pelo menos 2 territorios") == 0) {
        int count = 0;
        for (int i = 0; i < n; i++)
            if (strcmp(mapa[i].cor, corJogador) == 0)
                count++;
        return count >= 2;
    }

    // 2. "Eliminar todas as tropas da cor Vermelha"
    if (strcmp(missao, "Eliminar todas as tropas da cor Vermelha") == 0) {
        for (int i = 0; i < n; i++)
            if (strcmp(mapa[i].cor, "Vermelha") == 0 && mapa[i].tropas > 0)
                return 0;
        return 1;
    }

    // 3. "Ter 10 ou mais tropas em qualquer território"
    if (strcmp(missao, "Ter 10 ou mais tropas em qualquer territorio") == 0) {
        for (int i = 0; i < n; i++)
            if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas >= 10)
                return 1;
        return 0;
    }

    // 4. "Conquistar 1 territorio de cada cor existente no mapa"
    if (strcmp(missao, "Conquistar um territorio de cada cor") == 0) {
        int coresEncontradas = 0;

        for (int i = 0; i < n; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0)
                coresEncontradas++;
        }

        return (coresEncontradas >= 3);
    }

    // 5. "Reduzir algum território inimigo a 0 tropas"
    if (strcmp(missao, "Reduzir algum territorio inimigo a 0 tropas") == 0) {
        for (int i = 0; i < n; i++)
            if (mapa[i].tropas == 0)
                return 1;
        return 0;
    }

    return 0;
}

// =========================================================
// LIBERAR MEMÓRIA
// =========================================================
void liberarMemoria(struct Territorio* mapa, char** missoesJogadores, int jogadores) {
    free(mapa);

    for (int i = 0; i < jogadores; i++)
        free(missoesJogadores[i]);

    free(missoesJogadores);
}

// =========================================================
// MAIN
// =========================================================
int main() {
    srand(time(NULL));

    int n, jogadores;
    int turno = 1;

    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &n);
    getchar();

    printf("Quantos jogadores vao participar? (1 a 5) ");
    scanf("%d", &jogadores);
    getchar();

    // Alocação do mapa
    struct Territorio* mapa = calloc(n, sizeof(struct Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }

    cadastrarTerritorios(mapa, n);

    // ===============================
    // SISTEMA DE MISSÕES
    // ===============================

    char* missoesBase[5] = {
        "Conquistar pelo menos 2 territorios",
        "Eliminar todas as tropas da cor Vermelha",
        "Ter 10 ou mais tropas em qualquer territorio",
        "Conquistar um territorio de cada cor",
        "Reduzir algum territorio inimigo a 0 tropas"
    };

    // Missões dos jogadores (dinâmico)
    char** missoesJogadores = malloc(jogadores * sizeof(char*));

    // Controle de missões já usadas
    int usadas[5] = {0};

    // Atribuição aleatória sem repetição
    for (int i = 0; i < jogadores; i++) {

        missoesJogadores[i] = malloc(100 * sizeof(char));

        int sorteio;
        do {
            sorteio = rand() % 5;
        } while (usadas[sorteio] == 1);

        usadas[sorteio] = 1;

        atribuirMissao(missoesJogadores[i], missoesBase[sorteio]);

        printf("\nMissao do Jogador %d: %s\n", i + 1, missoesJogadores[i]);
    }

    int opcao;

    // ===============================
    // LOOP PRINCIPAL DO JOGO
    // ===============================
    do {
        printf("\n===== MENU =====\n");
        printf("1 - Exibir mapa\n");
        printf("2 - Realizar ataque (turno atual)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        if (opcao == 1) {
            exibirMapa(mapa, n);
        }
        else if (opcao == 2) {

            int jogadorDaVez = (turno - 1) % jogadores;
            printf("\n=== TURNO %d - JOGADOR %d ===\n", turno, jogadorDaVez + 1);
            printf("Missao: %s\n", missoesJogadores[jogadorDaVez]);

            exibirMapa(mapa, n);

            int a, d;

            printf("\nEscolha o numero do territorio ATACANTE: ");
            scanf("%d", &a); getchar();

            printf("Escolha o numero do territorio DEFENSOR: ");
            scanf("%d", &d); getchar();

            a--; d--;

            if (a < 0 || a >= n || d < 0 || d >= n) {
                printf("Indice invalido!\n");
                continue;
            }

            if (strcmp(mapa[a].cor, mapa[d].cor) == 0) {
                printf("Erro: nao pode atacar territorio da mesma cor!\n");
                continue;
            }

            if (mapa[a].tropas <= 0) {
                printf("O atacante nao possui tropas!\n");
                continue;
            }

            atacar(&mapa[a], &mapa[d]);

            // Verificação silenciosa da missão
            if (verificarMissao(missoesJogadores[jogadorDaVez], mapa, n, mapa[a].cor)) {
                printf("\n\n**************\n");
                printf("O JOGADOR %d CUMPRIU SUA MISSAO E VENCEU O JOGO!\n", jogadorDaVez + 1);
                printf("Missao: %s\n", missoesJogadores[jogadorDaVez]);
                printf("**************\n\n");
                break;
            }

            turno++;
        }

    } while (opcao != 0);

    liberarMemoria(mapa, missoesJogadores, jogadores);

    printf("\nPrograma finalizado!\n");
    return 0;
}