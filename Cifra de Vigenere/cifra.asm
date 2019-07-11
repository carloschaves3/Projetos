section .text
	; Publica o rotulo "cifra" como global para chamar a subrotina de outro programa
	global	cifra

cifra:
	; "Move o esp" para o topo da pilha para utilizacao de variaveis locais da subrotina
	push	ebp
	mov	ebp, esp

	; Salva registradores utilizados pela linguagem c
	push	ebx
	push	esi
	push 	edx	

	; Obtendo tamanho do arquivo original
	mov	eax, 19		; sys_lseek
	mov	ebx, [ebp+8]	; file_descriptor	
	mov	ecx, 0		 
	mov	edx, 2		; fim_do_arquivo
	int	80h
	; Move offset novamente para a primeira posicao
	mov	eax, 19		; sys_lseek
	mov	ebx, [ebp+8]	; file_descriptor
	mov	ecx, 0
	mov	edx, 0		; fim_do_arquivo
	int	80h
	; Atribui o tamanho do vetor a variavel "len_texto"
	dec	eax
	mov	[len_texto], eax

	; Le do arquivo para o vetor "texto" de tamanho 20000 bytes
	mov	eax, 3
	mov	ebx, [ebp+8]
	mov	ecx, texto
	mov	edx, [len_texto]
	int	80h

	; Caso arquivo nao contenha nada
	mov	eax, texto
	cmp	byte[eax], 0
	je	arquivo_vazio
	jmp	continua
arquivo_vazio:
	; Retorna registradores utilizados pela linguagem c para sair da subrotina
	pop	edi
	pop	esi
	pop	ebx
	; Volta a pilha ao estado inical e retorna ebp ao valor original
	mov	esp, ebp
	pop	ebp
	; Retorna 1 como sinal de arquivo vazio ..
	mov	eax, -1
	ret
continua:	

	; Antes de fazer qualquer coisa percorre os caracteres do vetor "texto"
	; copiados do arquivo de entrada procurando por caracteres especiais ..
	; Se encontrar retorna -1, ERRO.
	mov	eax, texto
	dec	eax
prox:
	inc	eax
	cmp	byte[eax], 0
	je	arquivo_ok
	cmp	byte[eax], 65
	jl	verifica_espaco_ou_enter
	cmp	byte[eax], 90
	jg	verifica_minuscula
	jmp	prox
verifica_espaco_ou_enter:
	cmp	byte[eax], 32
	je	prox
	cmp	byte[eax], 10
	je	prox
	jmp	caractere_invalido
verifica_minuscula:
	cmp	byte[eax], 97
	jl	caractere_invalido
	cmp	byte[eax], 122
	jg	caractere_invalido
	jmp	prox
caractere_invalido:
	; Retorna registradores utilizados pela linguagem c para sair da subrotina
	pop	edi
	pop	esi
	pop	ebx
	; Volta a pilha ao estado inicial e retorna ebp ao valor original
	mov	esp, ebp
	pop	ebp
	; Retorna -1, codigo de erro
	mov	eax, -1
	ret
arquivo_ok:

	; Percorre o vetor novamente verificando se existem somente espacos em branco e enters no arquivo
	mov	eax, texto
	dec	eax
proxima_posicao_:
	inc	eax
	cmp	byte[eax], 10
	je	proxima_posicao_
	cmp	byte [eax], 32
	je	proxima_posicao_
	cmp	byte [eax], 0
	jne	contem_algum_caractere
	; se nao der o jump acima significa q contem somente espacos e enters
	pop	edi
	pop	esi
	pop	ebx

	mov	esp, ebp
	pop	ebp

	mov	eax, -1
	ret
contem_algum_caractere:

	; Salva em um "vetor_auxiliar" as posicoes dos espacos e enters
	mov	eax, texto
	mov	ebx, vetor_aux_char
	mov	ecx, vetor_aux_pos
	mov	edx, 0
	mov	edi, 0
	mov	esi, 0
	dec	esi
	dec	edi
laco_salva:
	inc	esi
	cmp	byte [eax+esi], 0		
	je	fim_laco_salva
	cmp	byte [eax+esi], 32
	je	salva_posicao
	cmp	byte[eax+esi], 10
	je	salva_posicao
	jmp	laco_salva
salva_posicao:
	inc	edi
	mov	[ecx], esi
	add	ecx, 4
	mov	dl, [eax+esi]
	mov	[vetor_aux_char+edi], dl
	jmp	laco_salva
fim_laco_salva:
	inc	edi
	mov	byte [vetor_aux_char+edi], 0
	mov	[len_vetor_aux], edi	
	add	ecx, 4
	mov	dword [ecx], -1

	; Tirando espacos e enters do vetor "texto" para juntar os caracteres em uma unica linha
	mov	eax, texto
	dec	eax
	mov	ecx, -1
	mov	edx, 0
	mov	esi, 0
proxim:
	inc	eax
	inc	ecx
	cmp	byte[eax], 0
	je	fim_vetor
	cmp	byte[eax], 32
	je	proxim
	cmp	byte[eax], 10
	je	proxim
	mov	dx, [eax]
	mov	[texto+esi], dx
	inc	esi
	jmp	proxim
fim_vetor:
	mov	dword [len_texto], esi
	inc	esi
	mov	byte [texto+esi], 0
	
	; Copiando chave para um vetor de nome "chave"
	mov	eax, [ebp+16]
	mov	ebx, 0
	dec	eax
	dec	ebx
loop_chave:
	inc	eax
	inc	ebx
	cmp	byte [eax], 0
	je	fim_loop_chave
	mov	dx, [eax]
	mov	[chave+ebx], dx
	jmp	loop_chave
fim_loop_chave:
	mov	dx, [eax]
	mov	[chave+ebx], dx
	mov	dword[len_chave], ebx

	; Igualando tamanhos do "texto" e da "chave"
	mov	eax, texto
	mov	ebx, chave
	mov	ecx, [len_chave]
	mov	esi, -1
	mov	edi, -1
laco_tamanho:
	inc	eax
	inc	ebx
	inc	esi
	inc	edi
	cmp	byte [eax], 0
	je	fim_tamanho
	cmp	esi, ecx
	je	resseta_vetor
	mov	dx, [chave+esi]
	mov	[chave+edi], dx
	jmp	laco_tamanho
ressetado:
	mov	dx, [chave+esi]
	mov	[chave+edi], dx
	jmp	laco_tamanho
resseta_vetor:
	mov	esi, 0
	jmp	ressetado
fim_tamanho:
	mov	dx, 0
	mov	[chave+edi], dx
	inc	edi
	mov	[len_chave], edi

	; Passando "texto" e "chave" tudo para maiusculo para usar a logica da cifra
	mov	eax, texto
	mov	ebx, chave
	mov	esi, -1
	dec	eax
	dec	ebx
proximo_indice:	
	inc	eax
	inc	ebx
	inc	esi
	cmp	byte [eax], 0
	je	terminou_maiusculo
	mov	cx, [eax]
	mov	dx, [ebx]
	cmp	byte [eax], 97
	jl	vai_para_chave
	sub	cx, 32
	mov	[texto+esi], cx
vai_para_chave:
	cmp	byte [ebx], 97
	jl	proximo_indice
	sub	dx, 32
	mov	[chave+esi], dx
	jmp	proximo_indice
terminou_maiusculo:

	; Cifrando o "texto"
	mov	eax, texto
	mov	ebx, chave
	mov	esi, -1
laco_cifra:	
	inc	esi
	cmp	byte[eax+esi], 0
	je	fim_laco_cifra
	mov	cl, byte [eax+esi]
	mov	dl, byte [ebx+esi]
	add	cl, dl
	cmp	cl, 155
	jg	maior
	sub	cl, 65
	jmp	copia
maior:
	sub	cl, 91
	jmp	copia
copia:
	mov	[texto+esi], cl
	jmp	laco_cifra
fim_laco_cifra:

	; Devolve os espacos e enters para suas posicoes originais antes de gravar no arquivo de saida
	mov	eax, novo_cifrado
	mov	ebx, texto
	mov	edi, vetor_aux_char
	mov	esi, vetor_aux_pos
	mov	ecx, 0
laco_novo_cifrado:
	cmp	byte [ebx], 0
	je	copia_e_termina
	cmp	ecx, [esi]
	je	copia_vet_char
	mov	dl, byte [ebx]
	mov	byte [eax], dl
	inc	eax
	inc	ebx
	inc	ecx
	jmp	laco_novo_cifrado
copia_vet_char:
	mov	dl, byte[edi]
	mov	byte [eax], dl
	inc	eax
	inc	edi
	inc	ecx   ;talvez aqui nao
	add	esi, 4
	jmp	laco_novo_cifrado
copia_e_termina:
	mov	dl, byte[ebx]
	mov	byte [eax], dl
	dec	ecx
	mov	[len_novo_cifrado], ecx

	; Escrevendo o arquivo cifrado no arquivo de saida
	mov	eax, 4
	mov	ebx, [ebp+12]
	mov	ecx, novo_cifrado
	mov	edx, dword [len_novo_cifrado]
	int	80h

	; Retorna os valores inicias dos registradres utilizados pela linguagem c
	pop	edi
	pop	esi
	pop	ebx

	; Limpa a pilha
	mov	esp, ebp
	pop	ebp
	
	; Retorna 0, CIFRA REALIZADA.
	mov	eax, 0
	ret	

section .bss
	texto			resb	20000
	chave			resb	20000
	vetor_aux_char		resb	20000
	vetor_aux_pos		resd	20000
	novo_cifrado		resb	20000
	len_vetor_aux		resd	1
	len_texto		resd	1
	len_chave		resd	1
	len_novo_cifrado	resd	1
