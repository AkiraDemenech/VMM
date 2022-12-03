#include"fifo.h"#include<stdlib.h>
#include"fifo.h"


typedef struct fifo {
	int limit;
	LIST queue;
} fifo_manager;

MANAGER new_fifo (int size_limit) {
	fifo_manager * fm = malloc(sizeof(fifo_manager));
	fm->limit = size_limit;
	fm->queue = NULL;
	return fm;
}

int fifo_access (MANAGER subst_man, int address) {

	if(subst_man == NULL)
		return -3; // sem fifo
	
	fifo_manager * fm = (fifo_manager*) subst_man;

	LIST node = fm->queue;
	LIST prev = NULL;

	while(list_len(node) > 0) {
		prev = node;

		if(address == *(int *)list_value(node)) 
			return -1; // encontrada		

		node = list_get_next(node);
	}

	int subst = -2; // adicionada sem conflitos

	if(list_len(fm->queue) < fm->limit) 

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