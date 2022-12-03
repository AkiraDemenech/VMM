
// Virtual Memory Manager (Simulator)

#include<stdio.h>
#include"fifo.h"
#include"lru.h"

#define ARG_DESLOCAMENTO 'D'
#define ARG_TLB 'T'
#define ARG_PAG 'P'
#define ARG_QUADROS 'Q'
#define ARG_ALGORITMO 'A'
#define ARG_BACKING_STORE 'B'
#define ARG_ENTRADA 'E'

#define COD_ENTRADA 1
#define COD_QUADROS 2
#define COD_ALGORITMO 3
#define COD_BACKING_STORE 4
#define COD_DESLOCAMENTO 5
#define COD_TLB 6
#define COD_PAG 7

#define DESLOCAMENTO 8 // bits do endereço dentro de cada quadro/página (tamanho em bytes)


// quantidade de quadros da memória física simulada
#define QUADROS 128 

// quantidade de páginas na tabela 
#define PAGINAS 256

// quantidade de páginas na TLB
#define TLB 16

// algoritmos implementados:
#define FIFO 'F' 
#define LRU 'L'

typedef struct buffer {
	int page_number;
	int frame_number;
} tlb_slot;

 

int simulator (FILE * accesses, FILE * backing_store, 
				int frames, int pages, int tlb_size, int offset, 
				MANAGER physical_manager, int(*physical_replacement)(MANAGER,int),
				MANAGER tlb_manager, int(* tlb_replacement)(MANAGER, int)) {

	int frame_size = 1 << offset; 				
	int offset_mask = frame_size - 1;
	char physical_memory[frames][frame_size];
	
	int page_table[pages];
	tlb_slot tlb[tlb_size];

	int c, b, r, t;
	for(c = 0; c < pages; c++) 
		page_table[c] = -1;

	for(b = 0; b < tlb_size; b++)	
		tlb[b].page_number = -1;

	int page_number, frame_number, index, physical_address;	
	int available_frame = 0;

	while(!feof(accesses)) {
		fscanf(accesses, "%d", &c);

		if(c < 0) {

			printf("\n Table %d:\n", c);

			if(c < -1) {
				for(b = 0; b < pages; b++) 
					if(page_table[b] < 0) { // ausente
						if(c != -3)
							printf("  Page number: %d Virtual address: %d \n", b, b << offset);

					} else if(c != 2) // presente
						printf("  Page number: %d Virtual address: %d \n\t Frame number: %d Physical address: %d \n", b, b << offset, page_table[b], page_table[b] << offset);
				
			} else for(b = 0; b < tlb_size; b++) {
				if(tlb[b].page_number < 0)
					break;
				printf("  Page number: %d Virtual address: %d \n\t Frame number: %d Physical address: %d \n", tlb[b].page_number, tlb[b].page_number << offset, tlb[b].frame_number, tlb[b].frame_number << offset);
			}

			printf("\n");		
			continue;
		}

		index = c & offset_mask; 
		page_number = c >> offset;

		r = tlb_replacement(tlb_manager, page_number);
		t = page_number;
		if(r >= 0) 
			t = r;

		r = physical_replacement(physical_manager, page_number);	

		for(b = 0; b < tlb_size; b++) 
			if(tlb[b].page_number == t || tlb[b].page_number < 0)
				break;

		if(tlb[b].page_number == page_number) // TLB hit	
			frame_number = tlb[b].frame_number;
		else { // TLB miss
			if(page_table[page_number] < 0) { // Page-fault
				if(r < 0) {  
					frame_number = available_frame;
					available_frame++;
				} else { // Page-replacement
					frame_number = page_table[r];
					page_table[r] = -2;
					// TLB remove

				}
					
				fseek(backing_store, size * frame_number, SEEK_SET);
			} else frame_number = page_table[page_number];
			tlb[b].frame_number = frame_number;
			tlb[b].page_number = page_number;
		}	
			

		

		physical_address = (frame_number << offset) + index;

		printf("Virtual address: %d Physical address: %d Value: %d\n", c, physical_address, physical_memory[frame_number][index]);
	}

	return 0;
}

int main (int argc, char ** argv) {


	// -p
	int paginas = PAGINAS;

	// -q
	int quadros = QUADROS;

	// -t
	int tlb = TLB;

	// -d
	int deslocamento = DESLOCAMENTO;
 

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
				printf("\n   %s: ", argv[c]);
				switch(argv[c][1] + ((argv[c][1] >= 'a') ? ('A' - 'a') : (0))) {
					case ARG_DESLOCAMENTO:	
						printf("offset");
						a = COD_DESLOCAMENTO;
					break;	

					case ARG_ALGORITMO:
						printf("page-replacement policy");
						a = COD_ALGORITMO;//3;
					break;

					case ARG_TLB:
						printf("TLB size");
						a = COD_TLB;
					break;

					case ARG_QUADROS:
						printf("physical memory size (frames)");
						a = COD_QUADROS;//2;
					break;	

					case ARG_ENTRADA:
						printf("command and virtual addresses input");
						a = COD_ENTRADA;//1;
					break;	

					case ARG_BACKING_STORE:
						printf("backing store");
						a = COD_BACKING_STORE;//4;

				}
				printf("\t[%d]\n",a);
				continue;
			}

			

			switch(a) {
				case COD_ENTRADA://1:
					printf("Input file: %s", argv[c]);
					entrada = fopen(argv[c], "r");
				break;

				case COD_QUADROS://2:
					sscanf(argv[c], "%d", &quadros);
					printf("Physical memory: %d frame%s", quadros, (quadros == 1) ? ("") : ("s"));
				break;

				case COD_TLB:
					sscanf(argv[c], "%d", &tlb);
					printf("TLB: %d page%s", tlb, (tlb == 1) ? ("") : ("s"));
				break;

				case COD_ALGORITMO://3:
					algoritmo = argv[c][0];
					if(algoritmo >= 'a')
						algoritmo += 'A' - 'a';
					printf("Strategy: ");	
					switch(algoritmo) {
						case FIFO:
							printf("FIFO (first-in, first-out)");
						break;

						case LRU:	
							printf("LRU (least recently used)");
						break;

						default:
							printf("%s (%c unknown)", argv[c], algoritmo);	
					}	
					
				break;

				case COD_BACKING_STORE://4:
					backing_store = argv[c];
					printf("Process binary: %s", backing_store);
				break;		

				case COD_DESLOCAMENTO://5:
					sscanf(argv[c], "%d", &deslocamento);
					printf("Frames/pages: 2^%d byte%s ", deslocamento, (deslocamento == 1) ? ("") : ("s"));
					
				break;	
			}	

			printf("\n");

			a++;	
		}
	}

	printf("\n");

	if(entrada == NULL) {
		printf("Addresses input file opening error.\n");
		return -1;
	} 	

	FILE * bs = fopen(backing_store, "rb");
	if(bs == NULL) {
		printf("Backing store file opening error.\n");
		return -2;
	}

	MANAGER gerenciador_fisico;
	int (* substituir_fisico)(MANAGER, int); 

	MANAGER gerenciador_tlb;
	int (* substituir_tlb)(MANAGER, int);

	switch (algoritmo) {
		case FIFO:
			printf("FIFO\n");

			gerenciador_tlb = new_fifo(tlb);
			gerenciador_fisico = new_fifo(quadros);

			substituir_tlb = fifo_access;
			substituir_fisico = fifo_access;
		break;
	
		default:
		
	}

	return simulator(entrada, bs, quadros, paginas tlb, deslocamento, gerenciador_fisico, substituir_fisico, gerenciador_tlb, substituir_tlb);
}