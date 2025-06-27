#include "Set.h"       //inclui os Protótipos
#include "ArvoreAVL.h" //inclui os Protótipos
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição do tipo Set
struct set {
  ArvAVL *arv;
  int qtd;
  struct iterator *iter;
};

Set *criaSet() {
  Set *s = (Set *)malloc(sizeof(Set));
  if (s != NULL) {
    s->arv = cria_ArvAVL();
    s->qtd = 0;
    s->iter = NULL;
  }
  return s;
}

void liberaSet(Set *s) {
  if (s != NULL) {
    libera_ArvAVL(s->arv);

    struct iterator *no;
    while (s->iter != NULL) {
      no = s->iter;
      s->iter = s->iter->prox;
      free(no);
    }

    free(s);
  }
}

int insereSet(Set *s, int num) {
  if (s == NULL)
    return 0;

  if (insere_ArvAVL(s->arv, num)) {
    s->qtd++;
    return 1;
  } else
    return 0;
}

int removeSet(Set *s, int num) {
  if (s == NULL)
    return 0;

  if (remove_ArvAVL(s->arv, num)) {
    s->qtd--;
    return 1;
  } else
    return 0;
}

int tamanhoSet(Set *s) {
  if (s == NULL)
    return 0;

  return s->qtd;
}

int consultaSet(Set *s, int num) {
  if (s == NULL)
    return 0;

  return consulta_ArvAVL(s->arv, num);
}

void imprimeSet(Set *s) {
  if (s == NULL)
    return;

  emOrdem_ArvAVL(s->arv);
}

void beginSet(Set *s) {
  if (s == NULL)
    return;

  // Inicia o iterador da árvore AVL
  s->iter = NULL;
  iterator_ArvAVL(s->arv, &(s->iter));
}

int endSet(Set *s) {
  // Verifica se o iterador está no fim
  if (s == NULL || s->iter == NULL)
    return 1;
  return 0;
}

void nextSet(Set *s) {
  if (s == NULL || s->iter == NULL)
    return;

  // Mover o iterador para o próximo item
  s->iter = s->iter->prox;
}

void getItemSet(Set *s, int *num) {
  if (s == NULL || s->iter == NULL) {
    return;
  }
  *num = s->iter->valor;
}

Set *uniaoSet(Set *A, Set *B) {
  if (A == NULL || B == NULL)
    return NULL;
  int x;
  Set *C = criaSet();

  for (beginSet(A); !endSet(A); nextSet(A)) {
    getItemSet(A, &x);
    insereSet(C, x);
  }

  for (beginSet(B); !endSet(B); nextSet(B)) {
    getItemSet(B, &x);
    insereSet(C, x);
  }

  return C;
}

Set *interseccaoSet(Set *A, Set *B) {
  if (A == NULL || B == NULL)
    return NULL;
  int x;
  Set *C = criaSet();

  if (tamanhoSet(A) < tamanhoSet(B)) {
    for (beginSet(A); !endSet(A); nextSet(A)) {
      getItemSet(A, &x);
      if (consultaSet(B, x))
        insereSet(C, x);
    }
  } else {
    for (beginSet(B); !endSet(B); nextSet(B)) {
      getItemSet(B, &x);
      if (consultaSet(A, x))
        insereSet(C, x);
    }
  }
  return C;
}

Set *diferencaSet(Set *A, Set *B) {
  if (A == NULL || B == NULL) {
    return NULL;
  }

  int x;
  Set *C = criaSet();

  for (beginSet(A); !endSet(A); nextSet(A)) {
    getItemSet(A, &x);
    if (!consultaSet(B, x)) {
      insereSet(C, x);
    }
  }

  return C;
}

Set *avaliaExpressao(Set *left, char *operador, Set *right) {
  if (strcmp(operador, "AND") == 0) {
    return interseccaoSet(left, right);
  } else if (strcmp(operador, "OR") == 0) {
    return uniaoSet(left, right);
  } else if (strcmp(operador, "NOT") == 0) {
    return diferencaSet(left, right);
  }
  return NULL;
}
