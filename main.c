#include "Hash/TabelaHash.h"
#include "Set/Set.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void carregaCSVParaHash(Hash* ha, const char* nomeArquivo) {
    if (ha == NULL) return;

    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[1024];
    while (fgets(linha, sizeof(linha), arquivo)) {
        int id;
        char texto[256];

        // Supondo que o CSV tenha o formato: id, valor1, texto
        sscanf(linha, "%d,%*d,%255[^\n]", &id, texto);

        struct tweet novoTweet;
        novoTweet.id = id;
        strcpy(novoTweet.texto, texto);

        // Insere na hash
        insereHash_ComPalavras(ha, novoTweet);
    }

    fclose(arquivo);
}


// Função auxiliar para criar um conjunto a partir de uma palavra
Set *criarConjuntoPorPalavra(Hash *ha, const char *palavra) {
  Set *conjunto = criaSet();
  if (ha != NULL && palavra != NULL) {
         
    int *ids = buscaPorPalavra(ha, palavra);
    if (ids != NULL) {
      for (int i = 0; ids[i] != -1; i++) {
        insereSet(conjunto, ids[i]);
      }
    }
  }
  return conjunto;
}

void processar_arquivo(Set *set_ids, const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char line[256];

    // Ler linha por linha
    while (fgets(line, sizeof(line), file)) {
        // Quebrar a linha em partes (ID, lixo, texto)
        char *id = strtok(line, ",");
        strtok(NULL, ","); // Ignorar a parte do lixo
        char *texto = strtok(NULL, ",");

        if (id != NULL && texto != NULL) {
            // Verificar se o ID está no Set
            if (consultaSet(set_ids, atoi(id))) {
                printf("ID: %s -> Texto: %s\n", id, texto);
            }
        }
    }

    fclose(file);
}

// Função para interpretar e executar a consulta do usuário
void executarConsulta(Hash *ha, const char *consulta) {
  Set *resultado = NULL;
  char palavra1[50], palavra2[50], operador[4];
  int numPalavras;

  // Tenta ler o formato (palavra1 AND palavra2), (palavra1 OR palavra2), ou (palavra1 NOT palavra2)
  numPalavras = sscanf(consulta, "%49s %3s %49s", palavra1, operador, palavra2);

  if (numPalavras == 3) {
    printf("Consulta com duas palavras detectada: '%s %s %s'\n", palavra1, operador, palavra2);

    Set *conjunto1 = criarConjuntoPorPalavra(ha, palavra1);
    Set *conjunto2 = criarConjuntoPorPalavra(ha, palavra2);

    if (conjunto1 == NULL || conjunto2 == NULL) {
      printf("Erro ao criar conjuntos.\n");
    } else {
      if (strcmp(operador, "AND") == 0) {
        resultado = interseccaoSet(conjunto1, conjunto2);
        printf("Resultado da interseccao: ");
        processar_arquivo(resultado, "corpus.csv");
        printf("\n");
      } 
      else if (strcmp(operador, "OR") == 0) {
        resultado = uniaoSet(conjunto1, conjunto2);
        printf("Resultado da uniao: ");
        processar_arquivo(resultado, "corpus.csv");
        printf("\n");
      } 
      else if (strcmp(operador, "NOT") == 0) {
        resultado = diferencaSet(conjunto1, conjunto2);
        printf("Resultado da diferenca: ");
        processar_arquivo(resultado, "corpus.csv");
        printf("\n");
      }
    }

  } else if (numPalavras == 1) {
    printf("Consulta com uma palavra detectada: '%s'\n", palavra1);
    resultado = criarConjuntoPorPalavra(ha, palavra1);
    printf("Resultado da pesquisa unica: ");
    processar_arquivo(resultado, "corpus.csv");
  } else {
    printf("Formato de consulta inválido.\n");
    return;
  }
}

int main() {
  // Criando a tabela hash
  int TABLE_SIZE = 300;
  Hash *ha = criaHash(TABLE_SIZE);

  // Carregando dados do arquivo CSV e inserindo na hash
  carregaCSVParaHash(ha, "corpus.csv");
  // Exportando dados da hash para um arquivo CSV (para visualização)
  exportaHashParaCSV(ha,"exporta.csv");

  char consulta[256];

  // Laço de repetição principal
  while (1) {
    // Solicita a consulta do usuário
    printf("Digite 'sair' ou insira sua consulta (ex: (i AND want) ou (is)): ");
    fgets(consulta, sizeof(consulta), stdin);

    // Remove o caractere de nova linha
    consulta[strcspn(consulta, "\n")] = '\0'; 

    // Verifica se o usuário deseja sair
    if (strcmp(consulta, "sair") == 0) {
      printf("Fechando buscador...\n");
      break;
    }

    // Executa a consulta do usuário
    executarConsulta(ha, consulta);
  }

  // Libera a tabela hash
  liberaHash(ha);

  return 0;
}
