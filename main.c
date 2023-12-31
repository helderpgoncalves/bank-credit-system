#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAX_DESCRIPTION_LENGTH 100
#define MAX_INCOME_SOURCES 10

#define MAX_USERS 100          // Máximo de utilizadores que podem ser guardados na base de dados
#define MAX_USERNAME_LENGTH 50 // Máximo de caracteres que um username pode ter
#define MAX_NAME_LENGTH 50     // Máximo de caracteres que um nome pode ter
#define MAX_PASSWORD_LENGTH 50 // Máximo de caracteres que uma password pode ter

typedef enum
{
    ADMINISTRADOR,
    DECISOR
} UserType; // Tipo de utilizador

typedef struct
{
    char username[MAX_USERNAME_LENGTH];
    char nome[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    UserType tipo;
} Utilizador; // Utilizador

typedef struct
{
    Utilizador *users;
    int num_users;
} UserDatabase; // Base de dados de utilizadores

typedef struct
{
    int id;
    char nome_cliente[50];
    char descricao[MAX_DESCRIPTION_LENGTH];
    float montante_total;
    char fontes_rendimento[MAX_INCOME_SOURCES][50];
    int num_fontes_rendimento;
    char estado_decisao[20];
    char resultado_decisao[20];
    char data_decisao[20];
    char justificacao_decisao[100];
    char utilizador_decisor[50];
} PedidoCredito; // Pedido de crédito

typedef struct no
{
    PedidoCredito dados;
    struct no *proximo;
} No; // Nó da fila

typedef struct
{
    No *frente;
    No *tras;
} Fila; // Fila de pedidos de crédito

void inicializarFila(Fila *fila)
{
    fila->frente = NULL;
    fila->tras = NULL;
} // Inicializa a fila

/**
 * Verifica se a fila está vazia
 * @param fila Fila a verificar
 * @return 1 se a fila estiver vazia, 0 caso contrário
 */
int filaVazia(Fila *fila)
{
    return fila->frente == NULL;
}

/**
 * Enfileira um pedido de crédito
 * @param fila Fila onde o pedido de crédito vai ser enfileirado
 * @param pedido Pedido de crédito a enfileirar
 */
void enfileirar(Fila *fila, PedidoCredito pedido)
{
    No *novoNo = (No *)malloc(sizeof(No));
    novoNo->dados = pedido;
    novoNo->proximo = NULL;

    if (filaVazia(fila))
    {
        fila->frente = novoNo;
        fila->tras = novoNo;
    }
    else
    {
        fila->tras->proximo = novoNo;
        fila->tras = novoNo;
    }
}

/**
 * Desenfileira um pedido de crédito
 * @param fila Fila onde o pedido de crédito vai ser desenfileirado
 * @return Pedido de crédito desenfileirado
 */
PedidoCredito desenfileirar(Fila *fila)
{
    if (filaVazia(fila))
    {
        PedidoCredito pedidoVazio;
        pedidoVazio.id = -1;
        return pedidoVazio;
    }

    No *noRemovido = fila->frente;
    PedidoCredito pedido = noRemovido->dados;

    fila->frente = fila->frente->proximo;
    free(noRemovido);

    if (fila->frente == NULL)
    {
        fila->tras = NULL;
    }

    return pedido;
}

void listarPedidosPorAnalisar(Fila *fila)
{
    system("clear") || system("cls");
    printf("\nPedidos de credito por analisar:\n");
    No *noAtual = fila->frente;

    while (noAtual != NULL)
    {
        PedidoCredito pedido = noAtual->dados;

        if (strcmp(pedido.estado_decisao, "por analisar") == 0)
        {
            printf("ID: %d\n", pedido.id);
            printf("Nome do Cliente: %s\n", pedido.nome_cliente);
            printf("Descricao: %s\n", pedido.descricao);
            printf("Montante total: %.2f\n", pedido.montante_total);
            printf("Fontes de rendimento: ");
            for (int i = 0; i < pedido.num_fontes_rendimento; i++)
            {
                printf("%s", pedido.fontes_rendimento[i]);
                if (i < pedido.num_fontes_rendimento - 1)
                {
                    printf(", ");
                }
            }
            printf("\n");
            printf("----------------------------\n");
        }

        noAtual = noAtual->proximo;
    }
}

void listarPedidosAnalisados(Fila *fila)
{
    system("clear") || system("cls");
    printf("\nPedidos de credito analisados:\n");
    No *noAtual = fila->frente;

    while (noAtual != NULL)
    {
        PedidoCredito pedido = noAtual->dados;

        if (strcmp(pedido.estado_decisao, "analisado") == 0)
        {
            printf("ID: %d\n", pedido.id);
            printf("Nome do Cliente: %s\n", pedido.nome_cliente);
            printf("Descricao: %s\n", pedido.descricao);
            printf("Motivo: %s\n", pedido.justificacao_decisao);
            printf("Montante total: %.2f\n", pedido.montante_total);
            printf("Fontes de rendimento: ");
            for (int i = 0; i < pedido.num_fontes_rendimento; i++)
            {
                printf("%s", pedido.fontes_rendimento[i]);
                if (i < pedido.num_fontes_rendimento - 1)
                {
                    printf(", ");
                }
            }
            printf("\n");
            printf("Resultado: %s\n", pedido.resultado_decisao);
            printf("Data: %s\n", pedido.data_decisao);
            printf("Justificacao: %s\n", pedido.justificacao_decisao);
            printf("Utilizador: %s\n", pedido.utilizador_decisor);
            printf("----------------------------\n");
        }

        noAtual = noAtual->proximo;
    }
}

void inserirPedidoCredito(Fila *fila, UserDatabase *userDatabase)
{
    PedidoCredito pedido;

    system("clear") || system("cls");
    printf("\n--- Inserir Pedido de Crédito ---\n");
    printf("Nome do cliente: ");
    scanf("%s", pedido.nome_cliente);

    printf("Descrição: ");
    scanf("%s", pedido.descricao);

    // Read the montante_total input as a number
    printf("Montante total: ");
    while (scanf("%f", &pedido.montante_total) != 1)
    {
        printf("Valor inválido. Insira um número válido para o montante total: ");
        while (getchar() != '\n')
            ; // Clear input buffer
    }

    // Read the fontes_rendimento array
    printf("Número de fontes de rendimento: ");
    scanf("%d", &pedido.num_fontes_rendimento);
    printf("Digite os nomes das fontes de rendimento:\n");
    for (int i = 0; i < pedido.num_fontes_rendimento; i++)
    {
        scanf("%s", pedido.fontes_rendimento[i]);
    }

    strcpy(pedido.estado_decisao, "por analisar");

    // Enqueue the pedido into the filaPedidos
    enfileirar(fila, pedido);

    printf("\nPedido de crédito inserido com sucesso.\n");
    // Add a delay for demonstration purposes
    sleep(1);
}

void listarPedidosAprovados(Fila *fila)
{
    system("clear") || system("cls");
    printf("\nPedidos de credito analisados:\n");
    No *noAtual = fila->frente;

    while (noAtual != NULL)
    {
        PedidoCredito pedido = noAtual->dados;

        if (strcmp(pedido.estado_decisao, "analisado") == 0 && strcmp(pedido.resultado_decisao, "aprovado") == 0)
        {
            printf("ID: %d\n", pedido.id);
            printf("Nome do Cliente: %s\n", pedido.nome_cliente);
            printf("Descricao: %s\n", pedido.descricao);
            printf("Motivo: %s\n", pedido.justificacao_decisao);
            printf("Montante total: %.2f\n", pedido.montante_total);
            printf("Fontes de rendimento: ");
            for (int i = 0; i < pedido.num_fontes_rendimento; i++)
            {
                printf("%s", pedido.fontes_rendimento[i]);
                if (i < pedido.num_fontes_rendimento - 1)
                {
                    printf(", ");
                }
            }
            printf("\n");
            printf("Resultado: %s\n", pedido.resultado_decisao);
            printf("Data: %s\n", pedido.data_decisao);
            printf("Justificacao: %s\n", pedido.justificacao_decisao);
            printf("Utilizador: %s\n", pedido.utilizador_decisor);
            printf("----------------------------\n");
        }

        noAtual = noAtual->proximo;
    }
}

void listarPedidosCredito(Fila *filaPedidos)
{
    system("clear") || system("cls");
    printf("--- Listar Pedidos de Crédito ---\n");

    // Verificar se existem pedidos de crédito na fila
    if (filaVazia(filaPedidos))
    {
        printf("Não existem pedidos de crédito.\n");
        return;
    }

    // Percorrer a fila e exibir os detalhes de cada pedido de crédito
    No *noAtual = filaPedidos->frente;

    while (noAtual != NULL)
    {
        PedidoCredito pedido = noAtual->dados;

        printf("ID do pedido: %d\n", pedido.id);
        printf("Nome do cliente: %s\n", pedido.nome_cliente);
        printf("Descrição: %s\n", pedido.descricao);
        printf("Montante total: %.2f\n", pedido.montante_total);
        printf("Estado de decisão: %s\n", pedido.estado_decisao);
        printf("-------------------------------\n");

        noAtual = noAtual->proximo;
    }
}

void listarPedidosMontanteAcima(Fila *fila)
{
    float valor;
    printf("Digite o valor mínimo: ");
    scanf("%f", &valor);

    printf("\nPedidos de crédito com montante acima de %.2f:\n", valor);

    Fila tempFila;
    inicializarFila(&tempFila);

    while (!filaVazia(fila))
    {
        PedidoCredito pedido = desenfileirar(fila);

        if (pedido.montante_total > valor)
        {
            // Print the credit request details
            printf("Nome do cliente: %s\n", pedido.nome_cliente);
            printf("Descrição: %s\n", pedido.descricao);
            printf("Montante total: %.2f\n", pedido.montante_total);
            printf("Número de fontes de rendimento: %d\n", pedido.num_fontes_rendimento);
            printf("Estado da decisão: %s\n", pedido.estado_decisao);
            printf("Utilizador decisor: %s\n", pedido.utilizador_decisor);
            printf("\n");

            // Enqueue the pedido into the temporary queue
            enfileirar(&tempFila, pedido);
        }
    }

    // Restore the original queue by enqueueing back the temporary queue elements
    while (!filaVazia(&tempFila))
    {
        PedidoCredito pedido = desenfileirar(&tempFila);
        enfileirar(fila, pedido);
    }

    printf("Fim da lista.\n");
}

void listarPedidosPorDecisor(Fila *fila)
{
    char decisor[50];
    printf("Digite o nome do decisor: ");
    fgets(decisor, sizeof(decisor), stdin);
    decisor[strcspn(decisor, "\n")] = '\0'; // Remove newline character

    printf("\nPedidos de crédito analisados pelo decisor '%s':\n", decisor);

    Fila tempFila;
    inicializarFila(&tempFila);

    while (!filaVazia(fila))
    {
        PedidoCredito pedido = desenfileirar(fila);

        if (strcmp(pedido.estado_decisao, "analisado") == 0 && strcmp(pedido.utilizador_decisor, decisor) == 0)
        {
            // Print the credit request details
            printf("Nome do cliente: %s\n", pedido.nome_cliente);
            printf("Descrição: %s\n", pedido.descricao);
            printf("Montante total: %.2f\n", pedido.montante_total);
            printf("Número de fontes de rendimento: %d\n", pedido.num_fontes_rendimento);
            printf("Estado da decisão: %s\n", pedido.estado_decisao);
            printf("Resultado da decisão: %s\n", pedido.resultado_decisao);
            printf("Data da decisão: %s\n", pedido.data_decisao);
            printf("Justificação da decisão: %s\n", pedido.justificacao_decisao);
            printf("\n");

            // Enqueue the pedido into the temporary queue
            enfileirar(&tempFila, pedido);
        }
    }

    // Restore the original queue by enqueueing back the temporary queue elements
    while (!filaVazia(&tempFila))
    {
        PedidoCredito pedido = desenfileirar(&tempFila);
        enfileirar(fila, pedido);
    }

    printf("Fim da lista.\n");
}

void gerarRelatorioCreditosAnalisados(Fila *fila)
{
    FILE *arquivo;
    arquivo = fopen("relatorio_creditos_analisados.txt", "w");
    if (arquivo == NULL)
    {
        printf("Erro ao criar o arquivo de relatório.\n");
        return;
    }

    fprintf(arquivo, "Relatório de Créditos Analisados\n\n");

    Fila tempFila;
    inicializarFila(&tempFila);

    while (!filaVazia(fila))
    {
        PedidoCredito pedido = desenfileirar(fila);

        if (strcmp(pedido.estado_decisao, "analisado") == 0)
        {
            // Write the credit request details to the file
            fprintf(arquivo, "Nome do cliente: %s\n", pedido.nome_cliente);
            fprintf(arquivo, "Descrição: %s\n", pedido.descricao);
            fprintf(arquivo, "Montante total: %.2f\n", pedido.montante_total);
            fprintf(arquivo, "Número de fontes de rendimento: %d\n", pedido.num_fontes_rendimento);
            fprintf(arquivo, "Estado da decisão: %s\n", pedido.estado_decisao);
            fprintf(arquivo, "Resultado da decisão: %s\n", pedido.resultado_decisao);
            fprintf(arquivo, "Data da decisão: %s\n", pedido.data_decisao);
            fprintf(arquivo, "Justificação da decisão: %s\n", pedido.justificacao_decisao);
            fprintf(arquivo, "Utilizador decisor: %s\n\n", pedido.utilizador_decisor);

            // Enqueue the pedido into the temporary queue
            enfileirar(&tempFila, pedido);
        }
    }

    // Restore the original queue by enqueueing back the temporary queue elements
    while (!filaVazia(&tempFila))
    {
        PedidoCredito pedido = desenfileirar(&tempFila);
        enfileirar(fila, pedido);
    }

    fclose(arquivo);
    printf("Relatório de créditos analisados gerado com sucesso.\n");
}

void editarEstadoPedidoCredito(Fila *fila, UserDatabase *userDatabase)
{
    char nomeCliente[50];
    printf("Digite o nome do cliente do pedido de crédito: ");
    scanf("%s", nomeCliente);

    Fila tempFila;
    inicializarFila(&tempFila);

    int encontrado = 0;

    while (!filaVazia(fila))
    {
        PedidoCredito pedido = desenfileirar(fila);

        if (strcmp(pedido.nome_cliente, nomeCliente) == 0)
        {
            encontrado = 1;

            printf("Estado atual do pedido: %s\n", pedido.estado_decisao);
            printf("Digite o novo estado do pedido (por analisar, em análise, analisado): ");
            char novoEstado[20];
            fgets(novoEstado, sizeof(novoEstado), stdin);
            novoEstado[strcspn(novoEstado, "\n")] = '\0'; // Remove the newline character

            while (strcmp(novoEstado, "por analisar") != 0 &&
                   strcmp(novoEstado, "em análise") != 0 &&
                   strcmp(novoEstado, "analisado") != 0)
            {
                printf("Estado inválido. Insira um estado válido (por analisar, em análise, analisado): ");
                fgets(novoEstado, sizeof(novoEstado), stdin);
                novoEstado[strcspn(novoEstado, "\n")] = '\0';
            }

            strcpy(pedido.estado_decisao, novoEstado);

            if (strcmp(novoEstado, "analisado") == 0)
            {
                printf("Resultado da decisão (aprovado/reprovado): ");
                scanf("%s", pedido.resultado_decisao);

                printf("Justificação da decisão: ");
                scanf("%s", pedido.justificacao_decisao);

                printf("Utilizador decisor: ");
                scanf("%s", pedido.utilizador_decisor);

                // Get the current date and time
                time_t t = time(NULL);
                struct tm *current_time = localtime(&t);

                // Format the date as dd/mm/yyyy
                sprintf(pedido.data_decisao, "%02d/%02d/%04d",
                        current_time->tm_mday, current_time->tm_mon + 1, current_time->tm_year + 1900);
            }
        }

        enfileirar(&tempFila, pedido);
    }

    // Restore the original queue by enqueueing back the temporary queue elements
    while (!filaVazia(&tempFila))
    {
        PedidoCredito pedido = desenfileirar(&tempFila);
        enfileirar(fila, pedido);
    }

    if (!encontrado)
    {
        printf("Pedido de crédito não encontrado.\n");
    }
    else
    {
        printf("Estado do pedido de crédito editado com sucesso.\n");
    }
}

void editarDecisaoPedidoCredito(Fila *fila, UserDatabase *userDatabase)
{
    system("clear") || system("cls");
    printf("--- Editar Decisão de Pedido de Crédito ---\n");

    // Verificar se existem pedidos de crédito analisados
    int temPedidosAnalisados = 0;
    No *noAtual = fila->frente;

    while (noAtual != NULL)
    {
        PedidoCredito pedido = noAtual->dados;

        if (strcmp(pedido.estado_decisao, "analisado") == 0)
        {
            temPedidosAnalisados = 1;
            break;
        }

        noAtual = noAtual->proximo;
    }

    if (!temPedidosAnalisados)
    {
        printf("Não existem pedidos de crédito analisados.\n");
        return;
    }

    // Pedir o ID do pedido de crédito a editar
    int idPedido;
    printf("Digite o ID do pedido de crédito a editar: ");
    scanf("%d", &idPedido);

    // Procurar o pedido de crédito com o ID especificado
    noAtual = fila->frente;
    PedidoCredito *pedidoEditar = NULL;

    while (noAtual != NULL)
    {
        PedidoCredito *pedido = &noAtual->dados;

        if (pedido->id == idPedido && strcmp(pedido->estado_decisao, "analisado") == 0)
        {
            pedidoEditar = pedido;
            break;
        }

        noAtual = noAtual->proximo;
    }

    if (pedidoEditar == NULL)
    {
        printf("Não foi encontrado um pedido de crédito com o ID especificado.\n");
        return;
    }

    // Pedir a nova decisão e justificação
    char novaDecisao[20];
    printf("Digite a nova decisão (aprovado/rejeitado): ");
    scanf("%s", novaDecisao);

    char novaJustificacao[100];
    printf("Digite a nova justificação: ");
    scanf(" %[^\n]s", novaJustificacao);

    // Atualizar os campos do pedido de crédito
    strcpy(pedidoEditar->resultado_decisao, novaDecisao);
    strcpy(pedidoEditar->justificacao_decisao, novaJustificacao);

    // Obter o utilizador decisor atual
    Utilizador *utilizadorDecisor = NULL;
    Utilizador *users = userDatabase->users;
    int numUsers = userDatabase->num_users;

    for (int i = 0; i < numUsers; i++)
    {
        if (users[i].tipo == DECISOR)
        {
            utilizadorDecisor = &users[i];
            break;
        }
    }

    if (utilizadorDecisor == NULL)
    {
        printf("Erro: não foi encontrado um utilizador com o tipo DECISOR.\n");
        return;
    }

    // Obter a data e hora atuais
    time_t t = time(NULL);
    struct tm *dataHora = localtime(&t);
    strftime(pedidoEditar->data_decisao, sizeof(pedidoEditar->data_decisao), "%Y-%m-%d %H:%M:%S", dataHora);

    // Atualizar o utilizador decisor do pedido de crédito
    strcpy(pedidoEditar->utilizador_decisor, utilizadorDecisor->username);

    printf("Decisão do pedido de crédito editada com sucesso.\n");
}

void removerPedidoCredito(Fila *filaPedidos)
{
    int id;
    printf("Digite o ID do pedido de crédito que deseja remover: ");
    scanf("%d", &id);

    // Procurar o pedido de crédito na fila
    No *noAtual = filaPedidos->frente;
    No *noAnterior = NULL;

    while (noAtual != NULL)
    {
        if (noAtual->dados.id == id)
        {
            // Remover o pedido de crédito encontrado
            if (noAnterior == NULL)
            {
                // O pedido está no início da fila
                filaPedidos->frente = noAtual->proximo;
            }
            else
            {
                // O pedido está no meio ou final da fila
                noAnterior->proximo = noAtual->proximo;
            }

            // Liberar a memória do pedido removido
            free(noAtual);

            printf("Pedido de crédito removido com sucesso!\n");
            return;
        }

        noAnterior = noAtual;
        noAtual = noAtual->proximo;
    }

    printf("Pedido de crédito não encontrado!\n");
}

void procurarPedidoCreditoPorNome(Fila *filaPedidos)
{
    char nome[100];
    printf("Digite o nome do cliente: ");
    scanf("%s", nome);

    // Procurar o pedido de crédito pelo nome
    No *noAtual = filaPedidos->frente;
    int encontrados = 0;

    while (noAtual != NULL)
    {
        if (strcmp(noAtual->dados.nome_cliente, nome) == 0)
        {
            // Pedido encontrado com o nome correspondente
            printf("ID do pedido: %d\n", noAtual->dados.id);
            printf("Nome do cliente: %s\n", noAtual->dados.nome_cliente);
            printf("Valor do crédito: %.2f\n", noAtual->dados.montante_total);
            printf("-------------------------------\n");
            encontrados++;
        }

        noAtual = noAtual->proximo;
    }

    if (encontrados == 0)
    {
        printf("Nenhum pedido de crédito encontrado para o nome fornecido.\n");
    }
}

void criarNovoUtilizador(UserDatabase *userDatabase)
{
    system("clear") || system("cls");
    printf("--- Criar Novo Utilizador ---\n");

    // Verificar se a base de dados de utilizadores está cheia
    if (userDatabase->num_users == MAX_USERS)
    {
        printf("Erro: A base de dados de utilizadores está cheia.\n");
        return;
    }

    // Pedir os dados do novo utilizador
    Utilizador novoUtilizador;

    printf("Digite o username: ");
    scanf("%s", novoUtilizador.username);

    printf("Digite o nome: ");
    scanf("%s", novoUtilizador.nome);

    printf("Digite a password: ");
    scanf("%s", novoUtilizador.password);

    int tipoUtilizador;
    printf("Digite o tipo de utilizador (0 - Administrador, 1 - Decisor): ");
    scanf("%d", &tipoUtilizador);

    if (tipoUtilizador == 0)
    {
        novoUtilizador.tipo = ADMINISTRADOR;
    }
    else if (tipoUtilizador == 1)
    {
        novoUtilizador.tipo = DECISOR;
    }
    else
    {
        printf("Erro: Tipo de utilizador inválido.\n");
        return;
    }

    // Adicionar o novo utilizador à base de dados
    userDatabase->users[userDatabase->num_users] = novoUtilizador;
    userDatabase->num_users++;

    // Salvar a base de dados de utilizadores no arquivo binário
    FILE *file = fopen("users.bin", "wb");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo users.bin para escrita.\n");
        return;
    }

    fwrite(userDatabase->users, sizeof(Utilizador), userDatabase->num_users, file);
    fclose(file);

    printf("Novo utilizador criado com sucesso e guardado em users.bin.\n");
}

void exibirMenuAdmin(Fila *filaPedidos, UserDatabase *userDatabase)
{
    int opcao;

    do
    {
        printf("\n--- Menu Administrador ---\n");
        printf("1. Listar pedidos de crédito por analisar\n");
        printf("2. Listar pedidos de crédito analisados\n");
        printf("3. Listar todos os pedidos de crédito analisados e aprovados\n");
        printf("4. Listar todos os pedidos de crédito com montante acima de um valor\n");
        printf("5. Listar todos os pedidos de crédito analisados por um determinado decisor\n");
        printf("6. Gerar relatório com créditos analisados, ordenados por valor\n");
        printf("7. Inserir novo pedido de crédito\n");
        printf("8. Alterar estado de um pedido de crédito\n");
        printf("9. Alterar decisão de um pedido de crédito\n");
        printf("10. Apagar um pedido de crédito\n");
        printf("11. Procurar um pedido de crédito pelo nome do cliente\n");
        printf("12. Criar novo utilizador\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            listarPedidosPorAnalisar(filaPedidos);
            break;
        case 2:
            // Implementar listagem de pedidos analisados
            listarPedidosAnalisados(filaPedidos);
            break;
        case 3:
            // Implementar listagem de pedidos analisados e aprovados
            listarPedidosAprovados(filaPedidos);
            break;
        case 4:
            // Implementar listagem de pedidos com montante acima de um valor
            listarPedidosMontanteAcima(filaPedidos);
            break;
        case 5:
            // Implementar listagem de pedidos analisados por decisor
            listarPedidosPorDecisor(filaPedidos);
            break;
        case 6:
            // Implementar geração de relatório de créditos analisados
            gerarRelatorioCreditosAnalisados(filaPedidos);
            break;
        case 7:
            // Implementar inserção de novo pedido de crédito
            inserirPedidoCredito(filaPedidos, userDatabase);
            break;
        case 8:
            // Implementar alteração de estado de um pedido de crédito
            editarEstadoPedidoCredito(filaPedidos, userDatabase);
            break;
        case 9:
            // Implementar alteração de decisão de um pedido de crédito
            editarDecisaoPedidoCredito(filaPedidos, userDatabase);
            break;
        case 10:
            // Implementar remoção de um pedido de crédito
            removerPedidoCredito(filaPedidos);
            break;
        case 11:
            // Implementar procura de um pedido de crédito pelo nome do cliente
            procurarPedidoCreditoPorNome(filaPedidos);
            break;
        case 12:
            // Implementar criação de novo utilizador
            criarNovoUtilizador(userDatabase);
            break;
        case 0:
            printf("Saindo do sistema...\n");
            break;
        default:
            printf("Opção inválida. Tente novamente.\n");
            break;
        }
    } while (opcao != 0);
}

int verificarCredenciaisDecisor(char *username, char *password, UserDatabase *userDB)
{
    for (int i = 0; i < userDB->num_users; i++)
    {
        Utilizador user = userDB->users[i];
        if (strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0 && user.tipo == DECISOR)
        {
            return 1; // Credenciais corretas para um decisor
        }
    }
    return 0; // Credenciais inválidas ou usuário não é um decisor
}

void analisarProximoPedidoCredito(Fila *filaPedidos, UserDatabase *userDatabase)
{
    system("clear") || system("cls");
    printf("--- Análise do Próximo Pedido de Crédito ---\n");

    // Verificar se existem pedidos de crédito para analisar
    if (filaVazia(filaPedidos))
    {
        printf("Não existem pedidos de crédito para analisar.\n");
        return;
    }

    // Obter o utilizador decisor atual
    Utilizador *utilizadorDecisor = NULL;
    Utilizador *users = userDatabase->users;
    int numUsers = userDatabase->num_users;

    for (int i = 0; i < numUsers; i++)
    {
        if (users[i].tipo == DECISOR)
        {
            utilizadorDecisor = &users[i];
            break;
        }
    }

    if (utilizadorDecisor == NULL)
    {
        printf("Erro: não foi encontrado um utilizador com o tipo DECISOR.\n");
        return;
    }

    // Obter o próximo pedido de crédito da fila
    PedidoCredito pedido = desenfileirar(filaPedidos);

    // Atualizar os campos do pedido de crédito
    strcpy(pedido.estado_decisao, "em análise");
    strcpy(pedido.utilizador_decisor, utilizadorDecisor->username);

    // Obter a data e hora atuais
    time_t t = time(NULL);
    struct tm *dataHora = localtime(&t);
    strftime(pedido.data_decisao, sizeof(pedido.data_decisao), "%Y-%m-%d %H:%M:%S", dataHora);

    // Exibir os detalhes do pedido de crédito
    printf("ID do pedido: %d\n", pedido.id);
    printf("Nome do cliente: %s\n", pedido.nome_cliente);
    printf("Descrição: %s\n", pedido.descricao);
    printf("Montante total: %.2f\n", pedido.montante_total);
    printf("Fontes de rendimento:\n");
    for (int i = 0; i < pedido.num_fontes_rendimento; i++)
    {
        printf("- %s\n", pedido.fontes_rendimento[i]);
    }
    printf("-------------------------------\n");

    // Solicitar a decisão do decisor
    char decisao[20];
    printf("Digite a decisão (aprovado/rejeitado): ");
    scanf("%s", decisao);

    char justificacao[100];
    printf("Digite a justificação: ");
    scanf(" %[^\n]s", justificacao);

    // Atualizar os campos do pedido de crédito com a decisão do decisor
    strcpy(pedido.estado_decisao, "analisado");
    strcpy(pedido.resultado_decisao, decisao);
    strcpy(pedido.justificacao_decisao, justificacao);

    printf("Decisão registrada com sucesso.\n");

    // Enfileirar novamente o pedido de crédito após a análise
    enfileirar(filaPedidos, pedido);
}

void exibirMenuDecisor(Fila *filaPedidos, UserDatabase *userDatabase)
{
    int opcao;

    do
    {
        system("clear") || system("cls");
        printf("--- Menu do Decisor ---\n");
        printf("1. Analisar próximo pedido de crédito\n");
        printf("2. Listar todos os pedidos de crédito\n");
        printf("3. Procurar pedido de crédito por nome do cliente\n");
        printf("4. Editar decisão de pedido de crédito\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            analisarProximoPedidoCredito(filaPedidos, userDatabase);
            break;
        case 2:
            listarPedidosCredito(filaPedidos);
            break;
        case 3:
            procurarPedidoCreditoPorNome(filaPedidos);
            break;
        case 4:
            editarDecisaoPedidoCredito(filaPedidos, userDatabase);
            break;
        case 5:
            printf("Saindo do menu do Decisor...\n");
            break;
        default:
            printf("Opção inválida. Tente novamente.\n");
            break;
        }

        printf("Pressione Enter para continuar...");
        getchar();
        getchar();

    } while (opcao != 5);
}

int main()
{
    Fila filaPedidos;
    inicializarFila(&filaPedidos);

    UserDatabase userDB;
    userDB.num_users = 0;

    // Allocate memory for the users array
    userDB.users = (Utilizador *)malloc(MAX_USERS * sizeof(Utilizador));
    if (userDB.users == NULL)
    {
        printf("Erro ao alocar memória para a base de dados de utilizadores.\n");
        return 1; // Return with an error code
    }

    // Add users to the database
    Utilizador admin;
    strcpy(admin.username, "admin");
    strcpy(admin.password, "admin");
    admin.tipo = ADMINISTRADOR;
    userDB.users[userDB.num_users++] = admin;

    Utilizador decisor1;
    strcpy(decisor1.username, "decisor");
    strcpy(decisor1.password, "decisor");
    decisor1.tipo = DECISOR;
    userDB.users[userDB.num_users++] = decisor1;

    FILE *fp = fopen("users.bin", "wb");
    fwrite(&userDB, sizeof(UserDatabase), 1, fp);
    fclose(fp);

    int opcao;

    do
    {
        printf("\n--- Menu Principal ---\n");
        printf("1. Login como administrador\n");
        printf("2. Login como decisor\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            // Login como administrador
            printf("\n--- Login Administrador ---\n");
            char adminUsername[MAX_USERNAME_LENGTH];
            char adminPassword[MAX_PASSWORD_LENGTH];

            printf("Username: ");
            scanf("%s", adminUsername);
            printf("Password: ");
            scanf("%s", adminPassword);

            // Verificar as credenciais do administrador
            if (strcmp(adminUsername, "admin") == 0 && strcmp(adminPassword, "admin") == 0)
            {
                exibirMenuAdmin(&filaPedidos, &userDB);
            }
            else
            {
                printf("Credenciais inválidas. Tente novamente.\n");
            }
            break;
        case 2:
            // Login como decisor
            printf("\n--- Login Decisor ---\n");
            char decisorUsername[MAX_USERNAME_LENGTH];
            char decisorPassword[MAX_PASSWORD_LENGTH];

            printf("Username: ");
            scanf("%s", decisorUsername);
            printf("Password: ");
            scanf("%s", decisorPassword);

            // Verificar as credenciais do decisor
            if (verificarCredenciaisDecisor(decisorUsername, decisorPassword, &userDB))
            {
                exibirMenuDecisor(&filaPedidos, &userDB);
            }
            else
            {
                printf("Credenciais inválidas. Tente novamente.\n");
            }
            break;
        case 0:
            printf("Saindo do sistema...\n");
            break;
        default:
            printf("Opção inválida. Tente novamente.\n");
            break;
        }
    } while (opcao != 0);

    return 0;
}