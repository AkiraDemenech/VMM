
#include<stdlib.h>
#include "list.h"

typedef struct node {
	LIST next;
	LIST prev;
	void * value;
	
} list;

LIST list_node (int value_size) {
	list * l = malloc(sizeof(list));
	l->value = malloc(value_size);
	list_set_prev(l, NULL);
	list_set_next(l, NULL);
	return l;
}

void list_del (LIST node) {
	if(list_value(node) != NULL)
		free(list_value(node));
	if(node != NULL) 
		free(node);
}

void * list_value (LIST node) {
	return (node == NULL) ? (0) : (((list *) node)->value);
}



LIST list_get_prev (LIST this) {
	return (this == NULL) ? (NULL) : (((list *) this)->prev);
}

void list_set_prev (LIST this, LIST prev) {
	if(this != NULL) {
		
		list * l = (list *) this;		
		l->prev = prev;		

	}	

	if(prev != NULL && list_get_next(prev) != this)
		list_set_next(prev, this);
	
		
}

LIST list_get_next (LIST this) {
	return (this == NULL) ? (NULL) : (((list *) this)->next);
}

void list_set_next (LIST this, LIST next) {
	if(this != NULL) {
		
	
		list * l = (list *) this;
		
					
		l->next = next;



			
	}

	if(next != NULL && list_get_prev(next) != this) 
		list_set_prev(next, this);
	
}

