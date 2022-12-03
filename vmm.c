#include<stdio.h>
#include"fifo.h"
#include"lru.h"
#define QUADROS 128
#define FIFO 'F'
#define LRU 'L'

int main (int argc, char ** argv) {


	// -q
	int quadros = QUADROS;

	// -a
	char algoritmo = FIFO;

	// -b
	char * backing_store = "BACKING_STORE.bin";

	// -e
	FILE * entrada = stdin;

	if(argv != NULL) {
		int c, a;
		for(c = 1, a = 1; c < argc; c++) {
			if(argv[c] == NULL)
				continue;

			if(argv[c][0] == '-') {
				printf("\nExpectativa %s: ", argv[c]);
				switch(argv[c][1] + ((argv[c][1] >= 'a') ? ('A' - 'a') : (0))) {
					case 'A':
						printf("algoritmo");
						a = 3;
					break;

					case 'Q':
						printf("quadros");
						a = 2;
					break;	

					case 'E':
						printf("endereços lógicos e comandos");
						a = 1;
					break;	

					case 'B':
						printf("backing store");
						a = 4;

				}
				printf("\n");
				continue;
			}

			

			switch(a) {
				case 1:
					printf("Arquivo de entrada: %s", argv[c]);
					entrada = fopen(argv[c], "r");
				break;

				case 2:
					sscanf(argv[c], "%d", &quadros);
					printf("Memória virtual: %d quadros", quadros);
				break;

				case 3:
					algoritmo = argv[c][0];
					if(algoritmo >= 'a')
						algoritmo += 'A' - 'a';
					printf("Algoritmo ");	
					switch(algoritmo) {
						case FIFO:
							printf("FIFO");
						break;

						case LRU:	
							printf("LRU");
						break;

						default:
							printf("%s (%c desconhecido)", argv[c], algoritmo);	
					}	
					
				break;

				case 4:
					backing_store = argv[c];
					printf("Código binário do processo: %s", backing_store);
				break;		
			}	

			printf("\n");

			a++;	
		}
	}

	printf("\n");

	if(entrada == NULL) {
		printf("Erro ao abrir o arquivo de entrada.\n");
		return -1;
	} 	

	FILE * bs = fopen(backing_store, "rb");
	if(bs == NULL) {
		printf("Erro ao abrir o backing store.\n");
		return -2;
	}

	

	return  0;
}