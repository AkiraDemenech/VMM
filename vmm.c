
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
#define ARG_LOG 'L'

#define COD_ENTRADA 1
#define COD_QUADROS 2
#define COD_ALGORITMO 3
#define COD_BACKING_STORE 4
#define COD_DESLOCAMENTO 5
#define COD_TLB 6
#define COD_PAG 7
#define COD_LOG 8

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

 

int simulator (FILE * log, FILE * accesses, FILE * backing_store, 
				int frames, int pages, int tlb_size, int offset, 
				MANAGER physical_manager, int(*physical_replacement)(MANAGER,int),
				MANAGER tlb_manager, int(* tlb_replacement)(MANAGER,int), int(* tlb_removal)(MANAGER, int)) {

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
	int available_frame = 0, total = 0, last = 0, miss = 0, fault = 0;

	while(fscanf(accesses, "%d", &c) == 1) {
		
		

		if(c < 0) {

			printf("\nTable %d:\n", c);

			if(c < -1) {
				for(b = 0; b < pages; b++) 
					if(page_table[b] < 0) { // ausente
						if(c != -3)
							printf("  Page number: %d Virtual address: %d-%d \n", b, b << offset, (b << offset) + offset_mask);

					} else if(c != -2) // presente
						printf("  Page number: %d Virtual address: %d-%d Frame number: %d Physical address: %d-%d \n", b, b << offset, (b << offset) + offset_mask, page_table[b], page_table[b] << offset, (page_table[b] << offset) + offset_mask);
				
			} else for(b = 0; b < tlb_size; b++) 
				if(tlb[b].page_number >= 0)
					printf("  Page number: %d Virtual address: %d-%d Frame number: %d Physical address: %d-%d \n", tlb[b].page_number, tlb[b].page_number << offset, (tlb[b].page_number << offset) + offset_mask, tlb[b].frame_number, tlb[b].frame_number << offset, (tlb[b].frame_number << offset) + offset_mask);
					
			

			printf("\n");		
			continue;
		}

		total++;
		index = c & offset_mask; 
		page_number = c >> offset;

		r = tlb_replacement(tlb_manager, page_number);
		t = page_number;
		if(r >= 0) {
			t = r;
			r = -1;	
		}			


		for(b = 0; b < tlb_size; b++) {
			if(tlb[b].page_number == t)
				break;
			if(tlb[b].page_number < 0)	
				r = b;
		}		
		if(b >= tlb_size) {		
			if(r < 0) {
				fprintf(log, "#%d %d %d\n physical replacement return = %d\n target = %d\n page = %d\n", total,miss,fault,r, t, page_number);
				return -4;
			}	

			b = r;
		}	
		r = physical_replacement(physical_manager, page_number);	

	//	printf("Before = %d\n",b);
	//	printf("%p-%p\n",&tlb[b],sizeof(tlb_slot) + (&tlb[b]) - 1);
		if(tlb[b].page_number == page_number) // TLB hit	
			frame_number = tlb[b].frame_number;
		else { // TLB miss
			
			if(total - last > 1) {
				fprintf(log, " \n");
				if(total - last > 2) {
					if(total - last > 4)
						fprintf(log,"%d hits\n", total - last - 1);
					fprintf(log," \n");
				}
			}
			last = total;
			miss++;
			fprintf(log, "#%d\t[%d:\t%d.%d]\tTLB miss [%d] .%d ",total,c,page_number,index,t,b);
			if(page_table[page_number] < 0) { // Page-fault
				fault++;
				fprintf(log,"\tPage-fault");
				if(r < 0) {  
					frame_number = available_frame;
					available_frame++;
				} else { 	 // 	Page-replacement
					fprintf(log, "\tPage-replacement [%d] ",r);
					frame_number = page_table[r];
					page_table[r] = -2;
					fprintf(log, "- %d", tlb_removal(tlb_manager, r)); // TLB remove
					for(t = 0; t < tlb_size; t++)
						if(tlb[t].page_number == r) {
							fprintf(log, "\t .%d ", t);
							tlb[t].page_number = -2;
						//	break;
						}
				}
				fprintf(log, "\t(%d)", frame_number);
				page_table[page_number] = frame_number;	
				fseek(backing_store, frame_size * page_number, SEEK_SET);				 
				fread(physical_memory[frame_number], sizeof(char), frame_size, backing_store);	
			} else frame_number = page_table[page_number];
		//	printf("After = %d\n",b);
			if(b < 0)
				return -4;
			tlb[b].frame_number = frame_number;
			tlb[b].page_number = page_number;
			fprintf(log, "\n");
		}	
			

		

		physical_address = (frame_number << offset) + index;

		printf("Virtual address: %d Physical address: %d Value: %d\n", c, physical_address, physical_memory[frame_number][index]);
	}

	printf("\nPage-fault rate: %.03f%%\nTLB hit rate: %.03f%%\n\n", 100*fault/(float)total, 100*(1 - (miss/(float)total)));
	fprintf(log, "\nStatistics: \n\tTLB miss: %d \n\tPage-fault: %d \n\tTotal memory accesses: %d \n", miss, fault, total);

	return 0;
}

int main (int argc, char ** argv) {

	printf("\nVirtual Memory Manager simulator:\tcommand line arguments \n");


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

	// -l
	FILE * log = stderr;

	if(argv != NULL) {
		int c, a;
		for(c = 1, a = 1; c < argc; c++) {
			if(argv[c] == NULL)
				continue;

			if(argv[c][0] == '-') {
				printf("\n\t%s: ", argv[c]);
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

					case ARG_PAG:
						printf("virtual memory size (pages)");
						a = COD_PAG;
					break;	

					case ARG_LOG:
						printf("actions log");
						a = COD_LOG;
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

			printf("\t");

			switch(a) {
				case COD_LOG:
					printf("Log file: %s", argv[c]);
					log = fopen(argv[c], "w");
				break;	

				case COD_ENTRADA://1:
					printf("Input file: %s", argv[c]);
					entrada = fopen(argv[c], "r");
				break;

				case COD_QUADROS://2:
					sscanf(argv[c], "%d", &quadros);
					printf("Physical memory: %d frame%s", quadros, (quadros == 1) ? ("") : ("s"));
				break;

				case COD_PAG:
					sscanf(argv[c], "%d", &paginas);
					printf("Virtual memory: %d page%s", paginas, (paginas == 1) ? ("") : ("s"));
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
	MANAGER gerenciador_tlb;
	int (* substituir)(MANAGER, int);
	int (* remover)(MANAGER, int);

	fprintf(log, "\nVirtual Memory Manager simulator: \n");

	switch (algoritmo) {
		case FIFO:
			fprintf(log,"\tFIFO\n");

			gerenciador_tlb = new_fifo(tlb);
			gerenciador_fisico = new_fifo(quadros);

			remover = fifo_remove;
			substituir = fifo_access;
		break;

		case LRU:
			fprintf(log,"\tLRU\n");

			gerenciador_tlb = new_lru(tlb);
			gerenciador_fisico = new_lru(quadros);

			remover = lru_remove;
			substituir = lru_access;
		break;
	
		default:
			printf("Unknown page-replacement strategy.\n");
			return -3;
	}


	fprintf(log, "\t TLB: %d page%s\n \n", tlb, (tlb == 1) ? ("") : ("s"));
	fprintf(log, "\t %d page%s\n", paginas, (paginas == 1) ? ("") : ("s"));
	fprintf(log, "\t %d frame%s\n", quadros, (quadros == 1) ? ("") : ("s"));
	
	fprintf(log, " \n\t Offset: %d bit%s\n\n", deslocamento, (deslocamento == 1) ? ("") : ("s"));

	

	return simulator(log, entrada, bs, quadros, paginas, tlb, deslocamento, gerenciador_fisico, substituir, gerenciador_tlb, substituir, remover);
}