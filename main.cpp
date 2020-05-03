#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "lib_agenda.h"
#include "lista.h"

using namespace std;

// chama as funções
int main()
{
    setlocale(LC_ALL, "Portuguese");

    puts("Iniciando Aplicacao...");
    puts("Abrindo Arquivo para Leitura...");

    FILE *arquivoLeitura = fopen("agenda.dat", "r");

    if (arquivoLeitura == NULL)
    {
        puts("Erro na Leitura do Arquivo agenda.dat");
        return 1;
    }

    puts("Arquivo Aberto para Leitura");
    puts("Lendo Contatos do Arquivo...");

    // Criar lista para receber os contatos do arquivo
    Lista *contatos = cria_lista();

    while (1)
    {
        Contato *c = ler_contato(arquivoLeitura);
        if (c == NULL)
            break;
        adicionar(contatos, c);
    }

    puts("Contatos Lidos com Sucesso!");

    bool sair = false;
    while (!sair)
    {
        puts("\n\tInsira uma das Opcoes Seguintes:\n");
        puts("\t1 - Buscar por nome ou sobrenome");
        puts("\t2 - Buscar por Qualquer Coisa");
        puts("\t3 - Inserir Novo Contato");
        puts("\t0 - Sair da Aplicacao");

        char opcao;
        scanf(" %c%*c", &opcao);

        // Ponteiro que usa uma variavel que armazenar função busca
        Lista *(*buscar)(Lista *, const char *);
        char mensagem[100];

        if (opcao == '1')
        {
            buscar = buscarPorNomeSobrenome;
            //copiar string pra outra
            strcpy(mensagem, "\tInsira um Nome/Sobrenome a ser Buscado (Digite * para listar todos os contatos ou $ para voltar ao menu principal)");
        }
        else if (opcao == '2')
        {
            buscar = buscarPorDetalhes;
            strcpy(mensagem, "\tInsira um Valor a ser Buscado em Qualquer Atributo (Digite * para listar todos os contatos ou $ para voltar ao menu principal)");
        }

        switch (opcao)
        {
        case '1':
        case '2':
        {
            puts(mensagem);
            char valorBuscado[100];
            scanf(" %100[^\n]%*c", valorBuscado);

            if (strcmp(valorBuscado, "*") == 0)
                strcpy(valorBuscado, "");
            else if (strcmp(valorBuscado, "$") == 0)
            {
                break;
            }

            //Buscando os contatos pelo valor digitado e atribuindo o resultado na lista resultados
            Lista *resultados = buscar(contatos, valorBuscado);

            if (resultados->tamanho == 0)
            {
                puts("Nao foi Encontrado Nenhum Contato Relacionado com o Valor Digitado!");
                puts("Digite Enter para Continuar...");
                getchar();
                break;
            }

            ordena(resultados);

            int id = 1;
            for (Elemento *it = resultados->cabeca->proximo; it != NULL; it = it->proximo)
            {
                printf("[Identificador do Contato = %d]\n", id++);
                mostrarContato(it->valor);
            }

            bool voltar = false;
            while (!voltar)
            {
                puts("\n\t\tInsira uma das Opcoes Seguintes:\n");
                puts("\t\t1 - Editar um Contato");
                puts("\t\t2 - Remover um Contato");
                puts("\t\t0 - Voltar ao menu principal");

                char opcao;
                scanf(" %c%*c", &opcao);

                if (opcao == '1')
                {
                    int id;
                    bool repetir = false;
                    do
                    {
                        puts("\tInsira o Identificador do Contato a ser Alterado (Digite um numero menor ou igual a 0 (zero) para voltar ao menu anterior):");
                        scanf("%d%*c", &id);

                        if (id <= 0)
                            continue;

                        // busca contato com id igual ao digitado pelo usuario
                        Elemento *c = buscarPorPosicao(resultados, id);

                        if (c == NULL)
                        {
                            puts("Identificador invalido!");
                            repetir = true;
                        }
                        else
                        {
                            repetir = false;
                            alterarContato(c->valor);

                            id = 1;
                            for (Elemento *it = resultados->cabeca->proximo; it != NULL; it = it->proximo)
                            {
                                printf("[Identificador do Contato = %d]\n", id++);
                                mostrarContato(it->valor);
                            }
                        }
                    } while (repetir);
                }
                else if (opcao == '2')
                {
                    int id;
                    bool repetir = false;
                    do
                    {
                        puts("\tInsira o Identificador do Contato a ser Removido (Digite um numero menor ou igual a 0 (zero) para voltar ao menu anterior):");
                        scanf("%d%*c", &id);

                        if (id <= 0)
                            break;

                        // busca contato pelo id digitado pelo usuario pra poder excluir
                        Elemento *elementoARemover = buscarPorPosicao(resultados, id);

                        if (elementoARemover == NULL)
                        {
                            puts("Identificador Invalido");
                            repetir = true;
                        }
                        else
                        {
                            repetir = false;
                            //apaga contato da memoria
                            apagarPorContato(contatos, elementoARemover->valor);
                            //so remove da lista sem remover da memoria
                            removerPorContato(resultados, elementoARemover->valor);

                            puts("\tContato removido com sucesso! [Pressione Enter Para Continuar...]");
                            getchar();

                            id = 1;
                            for (Elemento *it = resultados->cabeca->proximo; it != NULL; it = it->proximo)
                            {
                                printf("[Identificador do Contato = %d]\n", id++);
                                mostrarContato(it->valor);
                            }
                        }
                    } while (repetir);
                }
                else if (opcao == '0')
                {
                    voltar = true;
                }
                else
                {
                    puts("Erro: Insira uma opcao valida!");
                    puts("Digite Enter para Continuar...");
                    getchar();
                }
            } // Fim While

            // remove lista da memoria sem apagar os contatos da memoria
            removerLista(resultados, resultados->cabeca);
        }
        break;

        case '3':
        {
            puts("\tInsira os dados do contato:");
            Contato *contato = obterContatoDeUsuario(); // exibe um formulario para o usuario preencher um novo contato

            // adiciona na lista o contato
            adicionar(contatos, contato);

            puts("Contato Adicionado com Sucessso!");
        }
        break;

        case '0':
            sair = true;
            break;

        default:
            puts("Erro: Insira uma opcao valida!");
            puts("Digite Enter para Continuar...");
            getchar();
        }
    }

    puts("Finalizando Aplicacao...");
    puts("Ordenando Contatos...");

    ordena(contatos);

    puts("Contatos Ordenados");
    puts("Fechando Arquivo de Leitura");

    fclose(arquivoLeitura);

    puts("Abrindo Arquivo para Escrita...");

    FILE *arquivoEscrita = fopen("agenda.dat", "w");

    if (arquivoEscrita == NULL)
    {
        puts("Erro na Escrita do Arquivo agenda.dat");
        return 2;
    }

    puts("Arquivo Aberto para Escrita");
    puts("Salvando Lista no Arquivo agenda.dat...");

    //pega a lista alterada e salva no arquivo
    salvarListaNoArquivo(contatos, arquivoEscrita);

    puts("Lista Salva no Arquivo agenda.dat");
    puts("Limpando Lista...");

    // remove lista e contatos da memoria
    apagarLista(contatos, contatos->cabeca);

    puts("Lista Limpa com Sucesso!");
    puts("Fechando Arquivo de escrita...");

    fclose(arquivoEscrita);

    puts("Arquivo Fechado com Sucesso!");
    puts("Ate breve! :)");

    return 0;
}
