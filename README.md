## Trabalho 1 da disciplina de Software Básico - 2023.1

### Alunos:
* Letícia de Moura Amaral - 200016750;
* Luiz Carlos da Silva Néto Vartuli - 200023314.

### SO Utilizado:
* Linux.

### Como compilar o programa:
* Foi feito um Makefile para facilitar a compilação;
* Comandos: 
  * 'make' ou 'make all': compila todos os programas e bibliotecas, já incluindo as interdependências entre eles;
  * 'make preprocessor': compila apenas o pré-processador;
  * 'make montador': compila apenas o montador;
  * 'make linker': compila apenas o montador;
  * 'make clean': limpa todos os arquivos de compilação para iniciar do zero.

### Como rodar o programa:
* './montador [file1] [file2] (...) [file n]' - Recebe arquivos '.asm' e retorna '.obj' ou '.exc':
  * Com BEGIN/END: './montador MOD_A MOD_B MOD_C' -> MOD_A.obj, MOD_B.obj, MOD_C.obj
  * Sem BEGIN/END: './montador MOD_A MOD_B MOD_C' -> MOD_A.exc, MOD_B.exc, MOD_C.exc
* './linker [file1] [file2] (...) [file n]' - Recebe arquivos '.obj' e retorna o '.exc' ligado:
  * './linker MOD_A MOD_B MOD_C' -> MOD_A.exc

### Observações:
* As linhas dos erros léxicos, sintáticos ou semânticos serão indicadas em relação ao arquivo pré-processado, não ao arquivo original.
