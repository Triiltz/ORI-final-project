// https://computinglife.wordpress.com/2008/11/20/why-do-hash-functions-use-prime-numbers/
// http://stackoverflow.com/questions/2624192/good-hash-function-for-strings

#include "TabelaHash.h" 
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define  MAX_IDS 100

// Definição do tipo Hash
struct hash {
  int qtd, TABLE_SIZE;
  struct tweet **itens;
};


Hash *criaHash(int TABLE_SIZE) {
  Hash *ha = (Hash *)malloc(sizeof(Hash));
  if (ha != NULL) {
    int i;
    ha->TABLE_SIZE = TABLE_SIZE;
    ha->itens = (struct tweet **)malloc(TABLE_SIZE * sizeof(struct tweet *));
    if (ha->itens == NULL) {
      free(ha);
      return NULL;
    }
    ha->qtd = 0;
    for (i = 0; i < ha->TABLE_SIZE; i++)
      ha->itens[i] = NULL;
  }
  return ha;
}

void liberaHash(Hash *ha) {
  if (ha != NULL) {
    int i;
    for (i = 0; i < ha->TABLE_SIZE; i++) {
      if (ha->itens[i] != NULL)
        free(ha->itens[i]);
    }
    free(ha->itens);
    free(ha);
  }
}

int valorString(char *str) {
  int i, valor = 7;
  int tam = strlen(str);
  for (i = 0; i < tam; i++)
    valor = 31 * valor + (int)str[i];
  return (valor & 0x7FFFFFFF);
}

/*
A partir daqui funções criadas para o caso específico do trabalho
*/

// Função para exportar os dados da tabela hash para um arquivo CSV
void exportaHashParaCSV(Hash *ha, const char *nomeArquivo) {
  if (ha == NULL)
    return;

  FILE *arquivo = fopen(nomeArquivo, "w");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo para exportação.\n");
    return;
  }

  // Percorrer a tabela hash
  for (int i = 0; i < ha->TABLE_SIZE; i++) {
    if (ha->itens[i] != NULL) {
      struct tweet *tt = ha->itens[i];

      // Para cada palavra no tweet, escreva no CSV
      for (int j = 0; j < tt->qtd_palavras; j++) {
        fprintf(arquivo, "%s,%d\n", tt->palavras[j], tt->id);
      }
    }
  }

  fclose(arquivo);
}

// Função para extrair palavras de uma string e ignorar símbolos, números ou espaços
void extrairPalavras(const char *frase, struct tweet *tw) {
  int i = 0, j = 0, k = 0;
  char palavra[20];

  while (frase[i] != '\0') {
    if (isalpha(frase[i])) {
      palavra[j++] = tolower(frase[i]);
    } else if (j > 0) {
      palavra[j] = '\0';
      strcpy(tw->palavras[k++], palavra);
      j = 0;
    }
    i++;
  }

  if (j > 0) {
    palavra[j] = '\0';
    strcpy(tw->palavras[k++], palavra);
  }
  tw->qtd_palavras = k;
}

// Função modificada de inserção para processar e armazenar cada palavra na hash
int insereHash_ComPalavras(Hash *ha, struct tweet tt) {
  if (ha == NULL || ha->qtd >= ha->TABLE_SIZE)
    return 0;

  int pos = valorString(tt.texto) % ha->TABLE_SIZE;
  while (ha->itens[pos] != NULL) {
    if (strcmp(ha->itens[pos]->texto, tt.texto) == 0) {
      return 0;
    }
    pos = (pos + 1) % ha->TABLE_SIZE;
  }

  // Armazena o tweet com palavras separadas
  ha->itens[pos] = (struct tweet *)malloc(sizeof(struct tweet));
  if (ha->itens[pos] == NULL)
    return 0;
  *ha->itens[pos] = tt;

  // Extrai e armazena as palavras
  extrairPalavras(tt.texto, ha->itens[pos]);
  ha->qtd++;
  return 1;
}

// Função para encontrar a palavra buscada e retornar a lista de ids     
int *buscaPorPalavra(Hash *ha, const char *palavra) {
    static int ids[MAX_IDS]; 
    int num_ids = 0;          

    // Limpar o vetor de IDs antes de usá-lo
    for (int i = 0; i < MAX_IDS; i++) {
        ids[i] = -1;
    }

    // Verificar se a tabela hash e a palavra são válidas
    if (ha == NULL || palavra == NULL) {
        printf("Hash ou palavra inválida.\n");
        return ids;
    }
    // Percorrer a tabela hash para encontrar os IDs correspondentes à palavra
    for (int i = 0; i < ha->TABLE_SIZE && num_ids < MAX_IDS; i++) {
        if (ha->itens[i] != NULL) {
            for (int j = 0; j < ha->itens[i]->qtd_palavras; j++) {
                if (strcmp(ha->itens[i]->palavras[j], (palavra )) == 0) {
                    ids[num_ids++] = ha->itens[i]->id;  // Armazena o ID no vetor
                    break; 
                }
            }
        }
    }

    if (num_ids == 0) 
        printf("Nenhum ID encontrado para a palavra '%s'.\n", palavra);
    
    ids[num_ids] = -1;
    return ids;
}