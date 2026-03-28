#include <stdio.h>
#include <string.h>

// Struct do território
struct Territorio {
    char nome[50];
    char cor[30];
    int tropas;
};

int main() {
    struct Territorio t[5];

    // Cadastro
    for (int i = 0; i < 5; i++) {
        printf("\nTerritorio %d\n", i + 1);

        printf("Nome: ");
        fgets(t[i].nome, 50, stdin);
        t[i].nome[strcspn(t[i].nome, "\n")] = 0; // remove \n

        printf("Cor: ");
        fgets(t[i].cor, 30, stdin);
        t[i].cor[strcspn(t[i].cor, "\n")] = 0;

        printf("Tropas: ");
        scanf("%d", &t[i].tropas);
        getchar(); // limpa o ENTER do teclado
    }

    // Mostrar resultado
    printf("\n--- RESULTADO ---\n");

    for (int i = 0; i < 5; i++) {
        printf("\nTerritorio %d\n", i + 1);
        printf("Nome: %s\n", t[i].nome);
        printf("Cor: %s\n", t[i].cor);
        printf("Tropas: %d\n", t[i].tropas);
    }

    return 0;
}