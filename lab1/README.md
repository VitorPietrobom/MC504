# MC504 - Lab 1

Documentação trabalho 1 de sistemas operacionais

	O projeto tem como objetivo ser o testador da corretude de sudoku. O programa recebe como input 81 números que são organizados em forma de matriz e tem como output a resposta de se a formatação do sudoku é válida.
	Lista de funções:
    • void recebeMatriz:
        ◦ input: int matriz[9][9] // matriz de int 9x9
        ◦ Recebe os 81 int’s para dentro da matriz
    • void printMatriz
        ◦ input: int matriz[9][9] // matriz de int 9x9
        ◦ Imprime a matriz de forma correta
    • void *h_thread:
        ◦ input: int arg[9][9] // matriz de int 9x9
        ◦ output: ret
        ◦ A função é responsável por verificar se as linhas do sudoku estão todas corretas
    • void *v_thread:
        ◦ input: int arg[9][9] // matriz de int 9x9
        ◦ output: ret
        ◦ A função é responsável por verificar se as colunas do sudoku estão todas corretas
    • void *c_thread:
        ◦ input: int arg[9][9] // matriz de int 9x9
        ◦ output: ret
          A função é responsável por verificar se as células do sudoku estão todas corretas
    • bool verifyTrue:
        ◦ input: bool arg[9]
        ◦ output: bool
        ◦ A função é responsável por verificar se todos os elementos de uma array de 9 elementos são iguais a true. Caso contrário, retorna false
