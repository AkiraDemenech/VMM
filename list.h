/**
 * @file list.h
 * @brief Lista duplamente ligada (estrutura de dados) 
 */
typedef void* LIST;

/**
 * @brief Aloca uma lista ligada 
 * @param value_size tamanho (em bytes) para o valor armazenado nessa posição na lista 	 
 * @returns o ponteiro para um nó de lista com o espaço interno informado 
*/
LIST list_node (int value_size);

/**
 * @brief Retorna o ponteiro para o próximo nó da lista
 * @param this nó atual da lista percorrida 
 * @returns o ponteiro para o próximo nó da lista
*/
LIST list_get_next (LIST this);


/**
 * @brief Atualiza o ponteiro para o próximo nó da lista
 * Chama também list_set_prev(next, this)
 * @param this o nó atual da lista 
 * @param next o próximo nó da lista
 * @return * void 
 */
void list_set_next (LIST this, LIST next);

LIST list_get_prev (LIST this);

void list_set_prev (LIST this, LIST prev);

void list_del (LIST node);

void *list_value (LIST node);









