
/**
 * @file fifo.c
 * @author Guilherme Akira Demenech Mori
 * @brief Implementação de \ref fifo.h utilizando filas \ref list.h
 * 
 * @details Armazenando em listas ligadas, 
 * forma uma fila em que novos acessos aguardam no final enquanto os mais antigos são substituídos 
 * 
 * @version 1
 * @date 2022-12-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include<stdlib.h>
#include"fifo.h"
#include"list.h"

/**
 * 
 * @struct fifo 
 * @brief Estrutura do gerenciador FIFO  
 * @details Definida também como o tipo \ref fifo_manager
 * (externamente utilizada como \ref MANAGER)
 * 
*/
typedef struct fifo {
	int limit;	/**< @short Quantidade máxima de endereços antes de aplicar substituições */ 
	LIST queue;	/**< @short Fila de inclusões recentes */ 
} fifo_manager;
/**
 * @brief Armazena o gerenciador FIFO
 * @typedef fifo_manager 
*/

MANAGER new_fifo (int size_limit) {
	fifo_manager * fm = malloc(sizeof(fifo_manager));
	fm->limit = size_limit;
	fm->queue = NULL;
	return fm;
}

int fifo_remove (MANAGER subst_man, int address) {
	if(subst_man == NULL)
		return -3;

	fifo_manager * fm = (fifo_manager*) subst_man;

	LIST node = fm->queue;	
	LIST prev = NULL;
	LIST next;
	int r = 0;

	while(node != NULL) {
		
		next = list_get_next(node);

		if(address == *(int *)list_value(node)) {
			if(prev == NULL)
				fm->queue = next;
			list_set_next(prev, next);
			list_del(node);
			r++;
		} else prev = node;					
		
		
		node = next;
	}

	
	return r;
}

int fifo_access (MANAGER subst_man, int address) {

	if(subst_man == NULL)
		return -3; // sem fifo
	
	fifo_manager * fm = (fifo_manager*) subst_man;

	LIST node = fm->queue;
	LIST prev = NULL;
	int i = 0;

	while(node != NULL) {
		prev = node;
		

		if(address == *(int *)list_value(node)) 
			return -1; // encontrada		

		node = list_get_next(node);
		i++;
	}

	int subst = -2; // adicionada sem conflitos

	if(i < fm->limit) 

		node = list_node(sizeof(int));								

	else {

		node = fm->queue;

		// segundo lugar assume
		fm->queue = list_get_next(fm->queue); 
		
		
		subst = * (int *) list_value(node);

		list_set_prev(fm->queue, NULL); // o novo primeiro 
		list_set_next(node, NULL); // primeiro vai ser o último

	}

	*(int*)list_value(node) = address;				 

	if(prev == NULL) 
		fm->queue = node;
	else list_set_next(prev, node);
	return subst;

}