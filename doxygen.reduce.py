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
		
with open('doxygen_config.txt', 'w', encoding='utf-8') as docs:
	print('#--------------------------------------------------------------------------- \n# Project related configuration options \n#--------------------------------------------------------------------------- \n', file=docs)
	
	comprimento = 4
	for d in dados:
		if len(d):
			config[d[0]] = d[1:]
			nomes.append(d[0])
			
			if len(d[0]) > comprimento:
				comprimento = len(d[0])
				
	for n in nomes:			
		print(n,' ' * (comprimento - len(n)), '=', *config[n], file=docs)
