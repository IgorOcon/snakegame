//------------------------------------------------------------------------------
// Definição e operações em uma fila genérica.
//------------------------------------------------------------------------------

#ifndef __QUEUE__
#define __QUEUE__

#ifndef NULL
#define NULL ((void *) 0)
#endif

//------------------------------------------------------------------------------
// estrutura de uma fila genérica, sem conteúdo definido.

typedef struct fila_t
{
   struct fila_t *prev ;  // aponta para o elemento anterior na fila
   struct fila_t *next ;  // aponta para o elemento seguinte na fila
} fila_t ;

//------------------------------------------------------------------------------
// Insere um elemento no final da fila.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - o elemento deve existir
// - o elemento nao deve estar em outra fila

void queue_append (fila_t **queue, fila_t *elem) ;

//------------------------------------------------------------------------------
// Remove o elemento indicado da fila, sem o destruir.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - a fila nao deve estar vazia
// - o elemento deve existir
// - o elemento deve pertencer a fila indicada
// Retorno: apontador para o elemento removido, ou NULL se erro

fila_t *queue_remove (fila_t **queue, fila_t *elem) ;

//------------------------------------------------------------------------------
// Conta o numero de elementos na fila
// Retorno: numero de elementos na fila

int queue_size (fila_t *queue) ;

//------------------------------------------------------------------------------
// Percorre a fila e imprime na tela seu conteúo. A impressão de cada
// elemento é feita por uma função externa, definida pelo programa que
// usa a biblioteca. Essa função deve ter o seguinte protótipo:
//
// void print_elem (void *ptr) ; // ptr aponta para o elemento a imprimir

void queue_print (char *name, fila_t *queue, void print_elem (void*) ) ;

#endif
