Orientações de uso dos arquivos ...

	O arquivo utilizado para cifragem é o "arquivo_original.txt"
	O arquivo escrito pela subrotina cifra é o "arquivo_cifrado.txt"
	O arquivo utilizado para decifragem é o "arquivo_cifrado.txt"
	O arquivo escrito pela subrotina decifra é o "arquivo_decifrado.txt"

	Para melhor experiência de testes utilize os seguintes comandos:
	
		nasm -f elf cifra.asm
		nasm -f elf decifra.asm
		
		gcc -m32 cifra.o cifra.c -o cifra.out
		* Para gerar um executável que realiza somente a cifragem.. *

		gcc -m32 decifra.o decifra.c -o decifra.out 
		* Para gerar um executável que realiza somente a decifragem.. *

		gcc -m32 cifra.o decifra.o teste.c -o cifra_e_decifra.out
		* Para gerar um executável que realiza a cifragem e a decifragem ....
		* A cifragem é realizada no "arquivo_original.txt" e o texto cifrado é escrito no "arquivo_cifrado.txt"
		* A decifragem é realizada no "arquivo_cifrado.txt" e o texto decifrado é escrito no "arquivo_decifrado.txt"
