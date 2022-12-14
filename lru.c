
/**
 * @file lru.c
 * @author Guilherme Akira Demenech Mori
 * @brief Implementação de \ref lru.h utilizando filas \ref list.h 
 * 
 * @details Armazenando em listas ligadas, 
 * forma uma fila em que acessos repetidos movem a prioridade para o final novamente 
 * 
 * @version 2
 * @date 2022-12-04
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#include<stdlib.h>
#include"lru.h"
#include"list.h"

/**
 * 
 * @struct lru 
 * @brief Estrutura do gerenciador LRU 
 * @details Definida também como o tipo \ref lru_manager
 * (externamente utilizada como \ref MANAGER)
 * 
*/
typedef struct lru {
	int limit;	/**< @short Quantidade máxima de endereços antes de aplicar substituições */
	LIST queue;	/**< @short Fila de acessos recentes */
	LIST last;	/**< @short Ponteiro para a posição na lista do último acesso */
} lru_manager;
/**
 * @brief Armazena o gerenciador LRU
 * @typedef lru_manager 
*/


MANAGER new_lru (int size_limit) {
	lru_manager * lm = malloc(sizeof(lru_manager));
	lm->limit = size_limit;
	lm->queue = NULL;
	lm->last = NULL;
	return lm;
}

int lru_remove (MANAGER replace_man, int address) {
	if(replace_man == NULL)
		return -3;

	lru_manager * lm = (lru_manager*) replace_man;

	LIST node = lm->queue;	
	LIST prev = NULL;
	LIST next;
	int r = 0;

	while(node != NULL) {
		
		next = list_get_next(node);

		if(address == *(int *)list_value(node)) {
			if(prev == NULL)
				lm->queue = next;
			if(next == NULL)	
				lm->last = prev;
			list_set_next(prev, next);
		//	list_del(node);
			r++;
		} else prev = node;					
		
		
		node = next;
	}

	
	return r;
}

int lru_access (MANAGER replace_man, int address) {
	if(replace_man == NULL)
		return -3; // sem fifo
	
	lru_manager * lm = (lru_manager*) replace_man;

	LIST node = lm->queue;
	LIST prev = NULL;
	LIST next;
	int i = 0;

	while(node != NULL) {
		
		next = list_get_next(node);

		if(address == *(int *)list_value(node)) {										
			if(next != NULL) { // /* 
				if(prev == NULL) 
					lm->queue = next;
				list_set_next(prev, next);
				list_set_next(node, NULL);
				list_set_next(lm->last, node);
				lm->last = node; // */
			}
			return -1; // encontrada		
		}	

		prev = node;
		node = next;
		i++;
	}

	int replace = -2; // adicionada sem conflitos

	if(i < lm->limit) 

		node = list_node(sizeof(int));								

	else {

		node = lm->queue;

		// segundo lugar assume
		lm->queue = list_get_next(lm->queue); 
		
		
		replace = * (int *) list_value(node);

		list_set_prev(lm->queue, NULL); // o novo primeiro 
		list_set_next(node, NULL); // primeiro vai ser o último

	}

	*(int*)list_value(node) = address;				 

	lm->last = node;
	if(prev == NULL) 
		lm->queue = node;
	else list_set_next(prev, node);
	return replace;
}