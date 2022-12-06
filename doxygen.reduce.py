'''
	Lê e reduz o arquivo de configuração do doxygen  
'''



dados = []
nomes = []
config = {}
with open('doconfig', 'r', encoding='utf-8') as dox:
	for ln in dox:
		if ln.isspace() or '#' in ln or not '=' in ln:
			print(ln.strip())	
			continue 
		dados.append([col.strip() for col in ln.split('=') if not col.isspace()])	
		
with open('doxygen_config', 'w', encoding='utf-8') as docs:
	
	
	comprimento = 4
	tamanho = 0
	for d in dados:
		if len(d):
			config[d[0]] = d[1:]
			nomes.append(d[0])
			
			if len(d[0]) > comprimento:
				comprimento = len(d[0])
			t = - len(d[0]) + 2
			for c in d:
				t += len(c) + 1
			if t > tamanho:	
				tamanho = t
				
	print('#','-' * (tamanho + comprimento), ' ', file=docs)			
	print('# Project related configuration options ', file=docs)
	print('#','-' * (tamanho + comprimento), ' ', file=docs)			
				
	for n in nomes:			
		print(n,' ' * (comprimento - len(n)), '=', *config[n], file=docs)
