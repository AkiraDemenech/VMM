#include<stdio.h>
#define QUADROS 128
#define FIFO 'F'
#define LRU 'L'

int main (int argc, char ** argv) {

	int quadros = QUADROS;
	char algoritmo = FIFO;
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

					
				}
				continue;
			}

			

			switch(a) {
				case 1:
					printf("\nArquivo de entrada: %s", argv[c]);
					entrada = fopen(argv[c], "r");
					
				break;

				case 2:
					sscanf(argv[c], "%d", &quadros);
					printf("\nMemória virtual: %d quadros", quadros);
				break;

				case 3:
					algoritmo = argv[c][0];
					if(algoritmo >= 'a')
						algoritmo += 'A' - 'a';
					printf("\nAlgoritmo ");	
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
			}	

			a++;	
		}
	}

	

	return  0;
}