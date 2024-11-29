//Criadores desse programa: 
//-FNatanaelRamos97 - https://github.com/FNatanaelRamos97
//-AndreasGomesMarchi - https://github.com/AndreasGomesMarchi
//-Ghlxavier - https://github.com/Ghlxavier
//-rafasilva41 - https://github.com/rafasilva41

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define FILMES 3
#define FILAS 5
#define COLUNAS 5
#define PRECO_INGRESSO 20.00  
#define PRECO_COMBO 15.00     

// Estrutura para armazenar detalhes do filme
typedef struct{
    char nome[50];
    char assentos[FILAS][COLUNAS];
} Filme;

// Estrutura para armazenar detalhes do bilhete
typedef struct {
    char nomeCliente[80];
    int filmeID, fila, coluna, bilheteID, combo, meiaEntrada;
    double valorIngresso; 
} Bilhete;

// Funções de interface e reserva
void Interface();

//Reserva 
void reservarBilhete();
void dadosCliente();
int MeiaEntrada();
void exibirFilmes();
void exibirAssentos();
void compra();
int combo();
void gerarBilhete();

//Cancelar bilhete
void cancelarBilhete();

// Impressão dos bilhetes reservados
void imprimirBilhetesReservados();
void imprimirBilhete();

// Funções para calcular totais e relatório final
void gerarRelatorioFinal();

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");

    Filme filmes[FILMES];
    strcpy(filmes[0].nome, "A volta dos que nao foram");
    strcpy(filmes[1].nome, "As trancas do Rei Careca");
    strcpy(filmes[2].nome, "Quem foi que disse?");

    for (int k = 0; k < FILMES; k++) {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            filmes[k].assentos[i][j] = 'D'; 
            }
        }
    }

    Bilhete bilhetes[100];
    int BilheteID = 100, numeroCadastro = 0;
    int totalIngressos = 0, totalCombos = 0;
    double totalArrecadado = 0.0;
    int opc;

    do {
        Interface();
        scanf("%d", &opc);

        switch (opc) {
            case 1:
                reservarBilhete(bilhetes, &numeroCadastro, filmes, &BilheteID, &totalIngressos, &totalCombos, &totalArrecadado);
            break;

            case 2:
                imprimirBilhetesReservados(bilhetes, numeroCadastro, filmes);
            break;

            case 3:
               cancelarBilhete(bilhetes, &numeroCadastro, filmes, &totalIngressos, &totalCombos, &totalArrecadado);
            break;

            case 4:
                gerarRelatorioFinal(totalIngressos, totalCombos, totalArrecadado);
            break;

            default:
                printf("Opcao invalida! O programa sera finalizado!\n");
            break;
        }
    } while (opc < 4 && opc != 0);

    return 0;
}

// Função para exibir a interface do menu
void Interface() {
    printf("-------------------------------------------------------------\n");
    printf("                   Bilheteria HollowMovie                    \n");
    printf("-------------------------------------------------------------\n\n");

    printf("Seja Bem-vindo ao melhor cinema de Hallownest!!!\n\n");
    printf("Escolha uma opcao da bilheteira:\n");
    printf("1 - Reservar lugar\n");
    printf("2 - Impressao do bilhete\n");
    printf("3 - Cancelar compra\n");
    printf("4 - Sair\n");
    printf("Escolha a opcao: ");
}



// Função para reservar bilhete
void reservarBilhete(Bilhete *bilhetes, int *numeroCadastro, Filme filmes[FILMES], int *BilheteID, int *totalIngressos, int *totalCombos, double *totalArrecadado) {
    int op = 1;

    do {
        exibirFilmes(filmes);

        // Escolha do filme
        printf("\nEscolha o filme\n");
        printf("Digite o seu codigo (de 1 a %d): ", FILMES);
        scanf("%d", &bilhetes[*numeroCadastro].filmeID);
        bilhetes[*numeroCadastro].filmeID -= 1; 

        // Captura os dados do cliente
        dadosCliente(&bilhetes[*numeroCadastro]);
        bilhetes[*numeroCadastro].meiaEntrada = MeiaEntrada();
        exibirAssentos(&filmes[bilhetes[*numeroCadastro].filmeID]);

        // Realiza a compra
        compra(&filmes[bilhetes[*numeroCadastro].filmeID], &bilhetes[*numeroCadastro]);
        bilhetes[*numeroCadastro].combo = combo();

        // Cálculo do valor final
        double valorIngresso = (bilhetes[*numeroCadastro].meiaEntrada ? PRECO_INGRESSO / 2 : PRECO_INGRESSO);
        bilhetes[*numeroCadastro].valorIngresso = valorIngresso + (bilhetes[*numeroCadastro].combo ? PRECO_COMBO : 0);

        // Exibir o valor final
        printf("\nValor Final a Ser Pago:\n");
        printf("Ingresso: R$ %.2f\n", valorIngresso);
        if (bilhetes[*numeroCadastro].combo) {
            printf("Combo (pipoca + refrigerante): R$ %.2f\n", PRECO_COMBO);
        }
        printf("Valor Total: R$ %.2f\n", bilhetes[*numeroCadastro].valorIngresso);

        // Gerar e exibir o ID do ingresso
        bilhetes[*numeroCadastro].bilheteID = ++(*BilheteID);
        printf("\nID do Ingresso: %d\n", bilhetes[*numeroCadastro].bilheteID); 

        (*numeroCadastro)++; 

        // Acumular os totais
        (*totalIngressos)++;
        if (bilhetes[*numeroCadastro - 1].combo) {
            (*totalCombos)++;
        }
        (*totalArrecadado) += bilhetes[*numeroCadastro - 1].valorIngresso;

        printf("\nDeseja realizar outra venda (1 - Sim; 2 - Nao): ");
        scanf("%d", &op);
        if(op == 2){
            printf("Obrigado por escolher a HollowMovie!!!\n\n");
        }

    } while (op == 1);
}

// Função para capturar os dados do cliente
void dadosCliente(Bilhete *bilhete) {
    printf("\nComo o cliente deseja ser chamado: ");
    scanf("%s", bilhete->nomeCliente);
}

// Função para definir meia entrada
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
            printf("\nOpcao invalida. Digite novamente.\n");
        }
    } while (opcao < 1 || opcao > 2);
    return -1;
}

// Função para exibir filmes disponíveis
void exibirFilmes(Filme filmes[FILMES]) {
    printf("\nFilmes em cartaz\n\n");
    for (int i = 0; i < FILMES; i++) {
        printf("%d - %s\n", i + 1, filmes[i].nome);
    }
}

// Função para exibir assentos disponíveis
void exibirAssentos(Filme *filme) {
    printf("\nAssentos Disponiveis (D para Disponivel, X para Ocupado):\n\n");

    printf("    "); 
    for (int i = 0; i < COLUNAS; i++) {
        printf("%2d ", i + 1);
    }
    printf("\n   ");
    for (int i = 0; i < COLUNAS; i++) {
        printf("----");
    }
    printf("\n");

    for (int i = 0; i < FILAS; i++) {
        printf("%2d | ", i + 1); 
        for (int j = 0; j < COLUNAS; j++) {
            printf(" %c ", filme->assentos[i][j]); 
        }
        printf("\n");
    }
}



// Função para realizar a compra (marcar assento como ocupado)
void compra(Filme *filme, Bilhete *bilhete) {
    int fila, coluna;
    do {
        printf("\nEscolha uma fila (1 a %d): ", FILAS);
        scanf("%d", &fila);
        printf("Escolha uma coluna (1 a %d): ", COLUNAS);
        scanf("%d", &coluna);

        if (filme->assentos[fila - 1][coluna - 1] == 'X') {
            printf("Esse assento ja foi reservado. Escolha outro.\n");
        } else {
            filme->assentos[fila - 1][coluna - 1] = 'X';
            bilhete->fila = fila - 1;
            bilhete->coluna = coluna - 1;
            printf("Assento reservado com sucesso!\n");
            break;
        }
    } while (1);
}

// Função para selecionar se o cliente deseja combo
int combo() {
    int opcao;
    printf("\nDeseja adicionar combo (pipoca + refrigerante) por R$ %.2f?\n", PRECO_COMBO);
    printf("1 - Sim; 2 - Nao: ");
    scanf("%d", &opcao);
    return opcao == 1 ? 1 : 0;
}



// Função para imprimir bilhetes reservados
void imprimirBilhetesReservados(Bilhete *bilhetes, int numeroCadastro, Filme filmes[FILMES]) {
	int i, numeroBilhete = 0, op = 1, selecionar = 0;
	
	do{
		printf("Selecione as opcao abaixo!\n\n");
		printf("1 - Imprimir todos os bilhetes.\n");
		printf("2 - Buscar bilhete por ID.\n\n");
		printf("Digite a opcao: ");
		scanf("%d", &selecionar);
		
		switch(selecionar){
			case 1:
				if(numeroCadastro == 0){
					printf("Sem bilhetes para pesquisa\n\n");
				}else{
					for (i = 0; i < numeroCadastro; i++) {
						Bilhete bilhete = bilhetes[i];
						imprimirBilhete(bilhete, filmes);
					}
				}
			break;

			case 2:
				if(numeroCadastro == 0){
					printf("Sem bilhetes para pesquisa\n\n");
				}else{
					do{
						printf("Digite ID do bilhete para a pesquisa: ");
						scanf("%d", &numeroBilhete);
						
					    for (i = 0; i < numeroCadastro; i++) {
					        Bilhete bilhete = bilhetes[i];
					        
					        if(bilhete.bilheteID == numeroBilhete){
					        	imprimirBilhete(bilhete, filmes);
						        
						        break;
							}else if(i == numeroCadastro-1){
								printf("Bilhete nao encontrado!\n\n");
							}
						}
					    
					    printf("Deseja pesquisar outro bilhete 1 - SIM e 2 - NAO: ");
					    scanf("%d", &op);
			
					}while(op == 1);
				}
			break;
				
			default:
				printf("Opcao nao encontrada!\n\n");
			break;
		}
		
		printf("Deseja fazer outra pesquisa: 1 - SIM e 2 - NAO: ");
		scanf("%d", &op);
	
    }while(op == 1);
	
}

void imprimirBilhete(Bilhete bilhete, Filme filmes[FILMES]){
		printf("\n-------- Bilhetes Reservado --------\n");
		printf("\nBilhete ID: %d\n", bilhete.bilheteID);
		printf("Cliente: %s\n", bilhete.nomeCliente);
		printf("Filme: %s\n", filmes[bilhete.filmeID].nome);
		printf("Assento: Fila %d, Coluna %d\n", bilhete.fila + 1, bilhete.coluna + 1);
		printf("Meia Entrada: %s\n", bilhete.meiaEntrada ? "Sim" : "Nao");
		printf("Combo: %s\n", bilhete.combo ? "Sim" : "Nao");
		printf("Valor Total: R$ %.2f\n", bilhete.valorIngresso);  
		printf("-------------------------------------\n\n"); 
}



// Função para cancelar bilhetes
void cancelarBilhete(Bilhete *bilhetes, int *numeroCadastro, Filme filmes[FILMES], int *totalIngressos, int *totalCombos, double *totalArrecadado) {
    int id, encontrado = 0;
    printf("\nInforme o ID do bilhete a ser cancelado: ");
    scanf("%d", &id);

    for (int i = 0; i < *numeroCadastro; i++) {
        if (bilhetes[i].bilheteID == id) {
            // Marcar o assento como disponível
            filmes[bilhetes[i].filmeID].assentos[bilhetes[i].fila][bilhetes[i].coluna] = 'D';

            // Atualizar os totais
            if (*totalIngressos > 0) {
                (*totalIngressos)--; 
            }
            if (bilhetes[i].combo && *totalCombos > 0) {
                (*totalCombos)--; 
            }
            if (*totalArrecadado >= bilhetes[i].valorIngresso) {
                (*totalArrecadado) -= bilhetes[i].valorIngresso; 
            } else {
                printf("\nErro: valor arrecadado e insuficiente para subtrair\n");
            }

            // Remover o bilhete cancelado
            for (int j = i; j < *numeroCadastro - 1; j++) {
                bilhetes[j] = bilhetes[j + 1];
            }
            (*numeroCadastro)--;

            printf("\nBilhete cancelado com sucesso!\n");
            printf("O valor do bilhete foi devolvido!");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("\nBilhete nao encontrado!\n");
    }
}



// Função para calcular e exibir o relatório final
void gerarRelatorioFinal(int totalIngressos, int totalCombos, double totalArrecadado) {
    printf("\n-------- Relatorio Final --------\n");
    printf("Total de ingressos vendidos: %d\n", totalIngressos);
    printf("Total de combos vendidos: %d\n", totalCombos);
    printf("Total arrecadado: R$ %.2f\n", totalArrecadado);
    printf("\nObrigado por escolher a HollowMovie!!!\n");
    printf("Volte Sempre!\n");
}
