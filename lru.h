/**
 * @file lru.h
 * 
 * @brief Interface com o gerenciador LRU de substituição de páginas 
 * 
 * 
 */


#include "vmm.h"

/**
 * @brief Aloca o gerenciador LRU com o tamanho limite informado 
 * 
 * @param size_limit quantidade máxima de endereços que podem ser armazenados ao mesmo tempo
 * @returns o ponteiro para o gerenciador LRU 
 */
MANAGER new_lru (int size_limit);

/**
 * @brief Se o endereço acessado for novo, verifica se há espaço para ele, informando qual página substituir se for preciso.
 * Se o endereço já estava na fila, move-o para o final novamente
 * 
 * @param replace_man o ponteiro para o gerenciador LRU
 * @param address o endereço acessado 
 * @returns o valor da página a ser substituída ou um valor negativo caso não haja substituição  
 */
int lru_access (MANAGER replace_man, int address);

/**
 * @brief Retira o endereço da fila, se ele estiver nela
 * 
 * @param replace_man o ponteiro para o gerenciador LRU 
 * @param address o endereço a ser removido
 * @returns a quantidade de aparições do endereço retiradas 
 */
int lru_remove (MANAGER replace_man, int address);

