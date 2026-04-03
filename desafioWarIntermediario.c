#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

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

void liberarMemoria(struct Territorio* mapa) {
    free(mapa);
}

int main() {
    srand(time(NULL));

    int n, jogadores;
    int turno = 1;

    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &n);
    getchar();

    printf("Quantos jogadores vao participar? ");
    scanf("%d", &jogadores);
    getchar();

    struct Territorio* mapa = (struct Territorio*) calloc(n, sizeof(struct Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }

    cadastrarTerritorios(mapa, n);

    int opcao;

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
            int jogadorDaVez = (turno - 1) % jogadores + 1;
            printf("\n=== TURNO %d - JOGADOR %d ===\n", turno, jogadorDaVez);

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

            turno++; // passa para o proximo jogador
        }

    } while (opcao != 0);

    liberarMemoria(mapa);

    printf("\nPrograma finalizado!\n");
    return 0;
}