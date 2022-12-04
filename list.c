
#include<stdlib.h>
#include "list.h"

typedef struct node {
	LIST next;
	LIST prev;
	void * value;
	int length;
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

int list_len (LIST node) {
	return (node == NULL) ? (0) : (((list *) node)->length);
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
		
		int last_len = l->length;	
		l->length = list_len(next) + 1;			
		l->next = next;

		if(last_len != l->length) {
			last_len = l->length;
			while(1) {			
				next = l;
				l = (list*) list_get_prev(l);

				if(l == NULL)
					return;

				last_len++;
				l->length = last_len;
			}
		}	
	}

	if(next != NULL && list_get_prev(next) != this) 
		list_set_prev(next, this);
	
}

