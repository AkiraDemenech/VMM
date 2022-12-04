/**
 * @file list.h
 * @brief Lista duplamente ligada (estrutura de dados) 
 */
typedef void* LIST;

/**
 * @brief Aloca e retorna um nó de lista com o espaço interno informado 
 * @param value_size tamanho (em bytes) do valor armazenado na lista 	 
*/
LIST list_node (int value_size);

/*!
	Retorna o ponteiro para o próximo nó da lista
*/
LIST list_get_next (LIST this);

//!
//! Atualiza o ponteiro para o próximo nó da lista
//!
void list_set_next (LIST this, LIST next);

LIST list_get_prev (LIST this);

void list_set_prev (LIST this, LIST prev);

void list_del (LIST node);

void *list_value (LIST node);









