// Potências de dois devem ser evitadas
// deve ser um número primo distante de pequenas potências de dois
#ifndef TABELAHASH_H
#define TABELAHASH_H

struct tweet {
  int id;
  char texto[256];
  char palavras[256][20]; // Armazena as palavras da frase
  int qtd_palavras;       // Quantidade de palavras na frase
};

typedef struct hash Hash;

Hash *criaHash(int TABLE_SIZE);
void liberaHash(Hash *ha);
int valorString(char *str);

void exportaHashParaCSV(Hash *ha, const char *nomeArquivo);

int insereHash_ComPalavras(Hash *ha, struct tweet tt);
void extrairPalavras(const char *frase, struct tweet *tw);
int *buscaPorPalavra(Hash *ha, const char *palavra);

#endif // TABELAHASH_H