/**
 * @file fifo.h
 * 
 * @brief Interface com o gerenciador FIFO de substituição de páginas 
 * 
 * 
 */


#include "vmm.h"

/**
 * @brief Aloca o gerenciador FIFO com o tamanho limite informado 
 * 
 * @param size_limit quantidade máxima de endereços que podem ser armazenados ao mesmo tempo
 * @returns o ponteiro para o gerenciador FIFO  
 */
MANAGER new_fifo (int size_limit);

/**
 * @brief Se o endereço acessado for novo, verifica se há espaço para ele, informando qual página substituir se for preciso. 
 * Não faz nada se o endereço já estiver na fila
 * 
 * @param subst_man o ponteiro para o gerenciador FIFO 
 * @param address o endereço acessado 
 * @returns o valor da página a ser substituída ou um valor negativo caso não haja substituição 
 */
int fifo_access (MANAGER subst_man, int address);

/**
 * @brief Retira o endereço da fila, se ele estiver nela
 * 
 * @param subst_man o ponteiro para o gerenciador FIFO 
 * @param address o endereço a ser removido  
 * @returns a quantidade de aparições do endereço retiradas  
 */
int fifo_remove (MANAGER subst_man, int address);