/**
 * @file list.h
 * @brief Lista duplamente ligada (estrutura de dados) 
 */

/**
 * @brief Tipo de ponteiro para lista 
 * 
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
 * @details Chama também \code{.c} list_set_prev(next, this) \endcode
 * @param this o nó atual da lista 
 * @param next o próximo nó da lista 
 */
void list_set_next (LIST this, LIST next);

/**
 * @brief Retorna o ponteiro para o nó anterior da lista
 * @param this nó atual da lista percorrida 
 * @returns o ponteiro para o nó anterior da lista 
 */
LIST list_get_prev (LIST this);

/**
 * @brief Atualiza o ponteiro para o nó anterior da lista
 * @details Chama também \code{.c} list_set_next(prev, this) \endcode 
 * @param this o nó atual da lista 
 * @param prev o nó anterior da lista 
 */
void list_set_prev (LIST this, LIST prev);

/**
 * @brief Desaloca a posição da lista  
 * 
 * @param node o nó da lista 
 */
void list_del (LIST node);

/**
 * @brief Acesso ao ponteiro alocado dentro do nó (com o tamanho informado na criação)
 * 
 * @param node o nó da lista 
 * @returns o ponteiro de armazenamento da posição da lista  
 */
void *list_value (LIST node);









