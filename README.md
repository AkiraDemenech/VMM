

# Simulador: Gerenciador de Memória Virtual

	

	Compilação: 
> `make`

> `make simulador`	

	Execução: 			
> `./simulador address.txt <quadros> <algoritmo> BACKING_STORE.bin <offset> <TLB> <páginas> arquivo.log`

- **address.txt**: arquivo com endereços lógicos e comandos 
> `stdin` por padrão

- **quadros**: quantidade de quadros disponíveis (número inteiro) 
> `128` por padrão 

- **algoritmo**: algoritmo de substituição de páginas utilizado (`"LRU"` ou `"FIFO"`) para a TLB e para a memória física 
> `"FIFO"` por padrão
			  
 - **BACKING_STORE.bin** (opcional): arquivo binário com os dados das páginas 
> `./BACKING_STORE.bin` por padrão
			  
 - **offset** (opcional): quantidade de bits menos significativos dos endereços que identifica um byte da página/do quadro (número inteiro) 
> `8` por padrão 

 - **TLB** (opcional): tamanho da TLB em páginas (número inteiro) 
> `16` por padrão 

 - **páginas** (opcional): quantidade de páginas (número inteiro) 
> `256` por padrão 

 - **arquivo.log** (opcional): arquivo para a exibição de TLB miss/hit, page-fault e page-replacement 
> `stderr` por padrão 

Os argumentos também podem ser informados em qualquer ordem, utilizando as opções:	

> `-e address.txt` 

> `-q <quadros>` 

> `-a <algoritmo>` 

> `-b BACKING_STORE.bin` 

> `-d <offset>` 

> `-t <TLB>` 

> `-p <páginas>` 

> `-l arquivo.log`


	Testes:

É possível gerar testes com o _script_ `random_accesses.py`

	Documentação:		

	

A `Makefile` tem os comandos necessários para gerar e limpar a documentação em HTML 

> `make doc`

`./html/index.hml` e `./latex/refman.pdf` são a documentação gerada 				 	