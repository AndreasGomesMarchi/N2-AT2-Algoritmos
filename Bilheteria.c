#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define FILMES 3
#define FILAS 5
#define COLUNAS 5
#define PRECO_INGRESSO 20.00  // Preço do ingresso inteiro
#define PRECO_COMBO 15.00     // Preço do combo (pipoca + refrigerante)

// Estrutura para armazenar detalhes do filme
typedef struct{
    char nome[50];
    char assentos[FILAS][COLUNAS];
} Filme;

// Estrutura para armazenar detalhes do bilhete
typedef struct {
    char nomeCliente[80];
    int filmeID, fila, coluna, bilheteID, combo, meiaEntrada;
} Bilhete;

// Funções de interface e reserva
void Interface();
void reservarBilhete();
void dadosCliente();
int MeiaEntrada();
void exibirFilmes(Filme filmes[FILMES]);
void exibirAssentos(Filme *filme);
void compra(Filme *filme, Bilhete *bilhete);
int combo();
void gerarBilhete();
void cancelarBilhete(Bilhete *bilhetes, int *numeroCadastro, Filme filmes[FILMES]);

// Impressão dos bilhetes reservados
void imprimirBilhetesReservados(Bilhete *bilhetes, int numeroCadastro, Filme filmes[FILMES]);

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");

    Filme filmes[FILMES];
    strcpy(filmes[0].nome, "A volta dos que não foram");
    strcpy(filmes[1].nome, "As tranças do Rei Careca");
    strcpy(filmes[2].nome, "Quem foi que disse?");
    
    // Inicializa os assentos como disponíveis para cada filme
    for (int k = 0; k < FILMES; k++) {
        for (int i = 0; i < FILAS; i++) {
            for (int j = 0; j < COLUNAS; j++) {
                filmes[k].assentos[i][j] = 'D';
            }
        }
    }

    Bilhete bilhetes[100];
    int BilheteID = 100, numeroCadastro = 0;
    int opc;

    do {
        Interface();
        scanf("%d", &opc);

        switch (opc) {
            case 1:
                reservarBilhete(bilhetes, &numeroCadastro, filmes, &BilheteID);
                break;

            case 2:
                imprimirBilhetesReservados(bilhetes, numeroCadastro, filmes);
                break;

            case 3:
                cancelarBilhete(bilhetes, &numeroCadastro, filmes);
                break;

            case 4:
                printf("Obrigado por escolher a HollowMovie!!!\n");
                printf("Volte Sempre!\n");
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    } while (opc < 4 && opc != 0);

    return 0;
}

void Interface() {
    printf("-------------------------------------------------------------\n");
    printf("                   Bilheteria HollowMovie                    \n");
    printf("-------------------------------------------------------------\n\n");

    printf("Seja Bem-vindo ao melhor cinema de Hallownest!!!\n\n");
    printf("Escolha uma opção da bilheteira:\n");
    printf("Reservar lugar - [1]\n");
    printf("Impressão do bilhete - [2]\n");
    printf("Cancelar compra - [3]\n");
    printf("Sair - [4]\n");
    printf("Escolha a opção: ");
}

// Reservar bilhete
void reservarBilhete(Bilhete *bilhetes, int *numeroCadastro, Filme filmes[FILMES], int *BilheteID) {
    int op = 1;

    do {
        exibirFilmes(filmes);

        // Escolha do filme
        printf("\nEscolha o filme\n");
        printf("Digite o seu código (de 1 a %d): ", FILMES);
        scanf("%d", &bilhetes[*numeroCadastro].filmeID);
        bilhetes[*numeroCadastro].filmeID -= 1; // Ajusta o índice

        // Captura os dados do cliente
        dadosCliente(&bilhetes[*numeroCadastro]);
        bilhetes[*numeroCadastro].meiaEntrada = MeiaEntrada();
        exibirAssentos(&filmes[bilhetes[*numeroCadastro].filmeID]);

        // Realiza a compra
        compra(&filmes[bilhetes[*numeroCadastro].filmeID], &bilhetes[*numeroCadastro]);
        bilhetes[*numeroCadastro].combo = combo();

        // Cálculo do valor final
        double valorIngresso = (bilhetes[*numeroCadastro].meiaEntrada ? PRECO_INGRESSO / 2 : PRECO_INGRESSO);
        double valorFinal = valorIngresso + (bilhetes[*numeroCadastro].combo ? PRECO_COMBO : 0);

        // Exibir o valor final
        printf("\nValor Final a Ser Pago:\n");
        printf("Ingresso: R$ %.2f\n", valorIngresso);
        if (bilhetes[*numeroCadastro].combo) {
            printf("Combo (pipoca + refrigerante): R$ %.2f\n", PRECO_COMBO);
        }
        printf("Valor Total: R$ %.2f\n", valorFinal);

        // Gerar e exibir o ID do ingresso
        bilhetes[*numeroCadastro].bilheteID = ++(*BilheteID);
        printf("\nID do Ingresso: %d\n", bilhetes[*numeroCadastro].bilheteID); // Exibe o ID do ingresso gerado

        (*numeroCadastro)++; // Incrementa o número de cadastros

        printf("\nDeseja realizar outra venda (1 - Sim; 2 - Nao): ");
        scanf("%d", &op);
        printf("Obrigado por escolher a HollowMovie!!!\n\n");

    } while (op == 1);
}

void dadosCliente(Bilhete *bilhete){
    printf("\nComo o cliente deseja ser chamado: ");
    scanf("%s", bilhete->nomeCliente);
}

int MeiaEntrada() {
    int opcao;
    do {
        printf("\nCliente deseja meia entrada?\n");
        printf("1 - Sim; 2 - Nao: ");
        scanf("%d", &opcao);
        if (opcao == 1) {
            return 1;
        } else if (opcao == 2) {
            return 0;
        } else {
            printf("\nOpção inválida. Digite novamente.\n");
        }
    } while (opcao < 1 || opcao > 2);
    return -1;
}

void exibirFilmes(Filme filmes[FILMES]) {
    printf("\nFilmes em cartaz\n\n");
    for (int i = 0; i < FILMES; i++) {
        printf("%d - %s\n", i + 1, filmes[i].nome);
    }
}

void exibirAssentos(Filme *filme) {
    printf("\nAssentos (D para Disponível e X para Ocupado):\n\n");
    printf("   ");
    for (int i = 0; i < COLUNAS; i++) {
        printf(" %d", i + 1);
    }
    printf("\n");
    for (int i = 0; i < FILAS; i++) {
        printf(" %d ", i + 1);
        for (int j = 0; j < COLUNAS; j++) {
            printf(" %c", filme->assentos[i][j]);
        }
        printf("\n");
    }
}

void compra(Filme *filme, Bilhete *bilhete) {
    int fila, coluna;

    do {
        printf("\nEscolha o assento\n");

        printf("Digite o número da fila: ");
        scanf("%d", &fila);
        fila = fila - 1;

        printf("\nDigite o valor da coluna: ");
        scanf("%d", &coluna);
        coluna = coluna - 1;

        printf("\n");

        if (fila >= 0 && fila < FILAS && coluna >= 0 && coluna < COLUNAS) {
            if (filme->assentos[fila][coluna] == 'D') {
                filme->assentos[fila][coluna] = 'X';
                bilhete->fila = fila;
                bilhete->coluna = coluna;
                printf("Assento reservado!\n");
                return;
            } else {
                printf("Assento ocupado. Selecione outra opção.\n");
            }
        } else {
            printf("Assento inválido. Selecione outra opção.\n");
        }
    } while (1);
}

int combo() {
    int opcao;
    printf("\nDeseja adicionar combo (pipoca + refrigerante)? (R$15,00)\n");
    printf("1 - Sim; 2 - Nao: ");
    scanf("%d", &opcao);
    return opcao == 1 ? 1 : 0;
}

// Função para cancelar bilhete
void cancelarBilhete(Bilhete *bilhetes, int *numeroCadastro, Filme filmes[FILMES]) {
    int bilheteID;
    printf("Digite o ID do bilhete a ser cancelado: ");
    scanf("%d", &bilheteID);

    for (int i = 0; i < *numeroCadastro; i++) {
        if (bilhetes[i].bilheteID == bilheteID) {
            filmes[bilhetes[i].filmeID].assentos[bilhetes[i].fila][bilhetes[i].coluna] = 'D'; // Marca o assento como disponível
            printf("Bilhete %d cancelado com sucesso!\n", bilheteID);

            // Remove o bilhete do array (move os elementos restantes para "baixo")
            for (int j = i; j < *numeroCadastro - 1; j++) {
                bilhetes[j] = bilhetes[j + 1];
            }

            (*numeroCadastro)--;  // Decrementa o número de cadastros
            return;
        }
    }

    printf("Bilhete não encontrado.\n");
}

// Imprimir todos os bilhetes reservados
void imprimirBilhetesReservados(Bilhete *bilhetes, int numeroCadastro, Filme filmes[FILMES]) {
    printf("\nBilhetes reservados:\n");
    for (int i = 0; i < numeroCadastro; i++) {
        printf("\n--------\n");
        printf("ID do ingresso: %d\n", bilhetes[i].bilheteID);
        printf("Cliente: %s\n", bilhetes[i].nomeCliente);
        printf("Filme: %s\n", filmes[bilhetes[i].filmeID].nome);
        printf("Assento: Fila %d, Coluna %d\n", bilhetes[i].fila + 1, bilhetes[i].coluna + 1);
    }
}
