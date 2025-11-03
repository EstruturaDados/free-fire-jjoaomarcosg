#include <stdio.h>      // Para entrada e saída (printf, scanf, getchar)
#include <stdlib.h>     // Para limpar a tela (system)
#include <string.h>     // Para manipulação de strings (strcpy, strcmp)

// --- Constantes Globais ---
#define CAPACIDADE_MOCHILA 10
#define MAX_NOME 30
#define MAX_TIPO 20

// --- Estrutura de Dados ---
// Define a struct para um item do inventário
struct Item {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
    int ocupado; // Flag para indicar se o "slot" está em uso (1 = sim, 0 = não)
};

// --- Protótipos das Funções ---
void inicializarMochila(struct Item mochila[], int tamanho);
void adicionarItem(struct Item mochila[], int tamanho);
void listarItens(const struct Item mochila[], int tamanho);
void removerItem(struct Item mochila[], int tamanho);
void limparBufferEntrada();
void pausarParaContinuar();

// --- Função Principal (main) ---
int main() {
    // 1. Setup: Cria a mochila e inicializa
    struct Item mochila[CAPACIDADE_MOCHILA];
    inicializarMochila(mochila, CAPACIDADE_MOCHILA);
    
    int escolha = 0;

    // 2. Game Loop: Menu interativo
    do {
        // No GitHub Codespaces (Linux), o comando para limpar a tela é "clear"
        system("clear"); 
        printf("--- GERENCIADOR DE MOCHILA (NIVEL NOVATO) ---\n");
        printf("Capacidade: %d slots\n", CAPACIDADE_MOCHILA);
        printf("----------------------------------------------\n");
        printf("1. Adicionar Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens\n");
        printf("0. Sair\n");
        printf("\nSua escolha: ");
        
        scanf("%d", &escolha);
        limparBufferEntrada(); // Limpa o '\n' do scanf

        switch (escolha) {
            case 1:
                adicionarItem(mochila, CAPACIDADE_MOCHILA);
                break;
            case 2:
                removerItem(mochila, CAPACIDADE_MOCHILA);
                break;
            case 3:
                listarItens(mochila, CAPACIDADE_MOCHILA);
                break;
            case 0:
                printf("\nSaindo da ilha... Boa sorte.\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                break;
        }

        if (escolha != 0) {
            pausarParaContinuar();
        }

    } while (escolha != 0);

    return 0;
}

// --- Implementação das Funções Utilitárias ---

/*
 * inicializarMochila(): Zera todos os slots da mochila.
 * Define a flag 'ocupado' como 0 (vazio) para todos os itens.
 */
void inicializarMochila(struct Item mochila[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        mochila[i].ocupado = 0; // Marca o slot como vazio
    }
}

/*
 * limparBufferEntrada(): Limpa o buffer do teclado (stdin)
 * para evitar problemas com scanf e fgets.
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * pausarParaContinuar(): Pausa a tela esperando o usuário.
 */
void pausarParaContinuar() {
    printf("\nPressione Enter para continuar...");
    getchar(); // Espera o usuário pressionar Enter
}

// --- Implementação da Lógica Principal ---

/*
 * adicionarItem(): Procura um slot vazio ('ocupado' == 0) e
 * preenche com os dados do novo item.
 */
void adicionarItem(struct Item mochila[], int tamanho) {
    int i;
    int slotEncontrado = 0;

    // 1. Encontrar um slot vazio
    for (i = 0; i < tamanho; i++) {
        if (mochila[i].ocupado == 0) {
            // Encontramos um slot! Vamos usá-lo.
            printf("\n--- Adicionando novo item ---\n");
            
            printf("Digite o nome do item: ");
            // fgets é mais seguro que scanf para strings, pois lê espaços
            // e evita buffer overflow.
            fgets(mochila[i].nome, MAX_NOME, stdin);
            // Remove o '\n' que o fgets captura no final
            mochila[i].nome[strcspn(mochila[i].nome, "\n")] = 0;

            printf("Digite o tipo do item (arma, cura, municao): ");
            fgets(mochila[i].tipo, MAX_TIPO, stdin);
            mochila[i].tipo[strcspn(mochila[i].tipo, "\n")] = 0;

            printf("Digite a quantidade: ");
            scanf("%d", &mochila[i].quantidade);
            limparBufferEntrada(); // Limpa o '\n' do scanf

            mochila[i].ocupado = 1; // Marca o slot como ocupado
            slotEncontrado = 1;
            
            printf("\nItem '%s' adicionado com sucesso!\n", mochila[i].nome);
            break; // Sai do loop 'for' pois já encontramos um slot
        }
    }

    // 2. Verificar se a mochila está cheia
    if (slotEncontrado == 0) {
        printf("\nERRO: Mochila cheia! Nao foi possivel adicionar o item.\n");
    }
}

/*
 * listarItens(): Percorre a mochila e imprime apenas os slots
 * que estão marcados como 'ocupado' == 1.
 */
void listarItens(const struct Item mochila[], int tamanho) {
    int i;
    int itensNaMochila = 0;

    printf("\n--- Itens na Mochila ---\n");
    printf("Slot | Nome           | Tipo       | Qtd.\n");
    printf("--------------------------------------------\n");

    for (i = 0; i < tamanho; i++) {
        if (mochila[i].ocupado == 1) {
            // Imprime formatado (como no projeto WAR)
            printf("%-4d | %-14s | %-10s | %-4d\n",
                   i + 1, // Slot amigável (1-10)
                   mochila[i].nome,
                   mochila[i].tipo,
                   mochila[i].quantidade);
            itensNaMochila++;
        }
    }

    if (itensNaMochila == 0) {
        printf("A mochila esta vazia.\n");
    }
    printf("--------------------------------------------\n");
} // <-- A CHAVE QUE FALTAVA ESTÁ AQUI

/*
 * removerItem(): Pede um nome ao usuário, procura pelo item
 * e, se encontrar, "remove" ele (setando 'ocupado' = 0).
 */
void removerItem(struct Item mochila[], int tamanho) {
    char nomeParaRemover[MAX_NOME];
    int i;
    int itemEncontrado = 0;

    printf("\n--- Removendo Item ---\n");
    printf("Digite o nome do item que deseja remover: ");
    fgets(nomeParaRemover, MAX_NOME, stdin);
    nomeParaRemover[strcspn(nomeParaRemover, "\n")] = 0; // Limpa o '\n'

    // Procura o item na mochila
    for (i = 0; i < tamanho; i++) {
        // Verifica se o slot está ocupado E se o nome bate (strcmp == 0)
        if (mochila[i].ocupado == 1 && strcmp(mochila[i].nome, nomeParaRemover) == 0) {
            mochila[i].ocupado = 0; // "Remove" o item
            itemEncontrado = 1;
            printf("\nItem '%s' removido com sucesso!\n", nomeParaRemover);
            break; // Para o loop pois já removemos
        }
    }

    if (itemEncontrado == 0) {
        printf("\nERRO: Item '%s' nao encontrado na mochila.\n", nomeParaRemover);
    }
}