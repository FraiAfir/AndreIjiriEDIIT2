# Makefile para o projeto TED - Trabalho de Estruturas de Dados
# Autor: André Felipe Ijiri Ribeiro

PROJ_NAME   = ted
TESTS_NAME  = ted_runner
all: $(TESTS_NAME) $(PROJ_NAME)

ALUNO = Andre_Ijiri # Nome do aluno (sem espaços, para uso em nomes de arquivos)



# =========================  DETECÇÃO AUTOMÁTICA DO SISTEMA OPERACIONAL ========================= #
ifeq ($(OS),Windows_NT)
    # Comandos para Windows (CMD/PowerShell)
    RM   = del /Q /F
    EXEC = .exe
	LIBS = -lm -lssp
# -lm:   Biblioteca matemática para funções como sqrt, pow, etc.
# -lssp: Biblioteca para proteção contra estouro de pilha (stack protector)
else
    # Comandos para Linux / macOS / WSL
    RM   = rm -f
    EXEC = 
	LIBS = -lm
# -lm:   Biblioteca matemática para funções como sqrt, pow, etc.
endif
# =============================================================================================== #





# ===================================== COMPILADOR E FLAGS ====================================== #
# Compilador
CC = gcc
# Flags
CFLAGS  = -g -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration -Wall
LDFLAGS = 

# Arquivos-fonte e objetos
OBJETOS = main.o params.o
TESTS   = ../test/test_params.c
SRCS    = params.c
# =============================================================================================== #





# ==================================== REGRAS DE COMPILAÇÃO ===================================== #
# Arquivos de teste
$(TESTS_NAME): $(TESTS) $(SRCS)
	@echo.

	@echo "Compilando os arquivos de teste $(TESTS_NAME): ..."
	$(CC) -o ../test/$(TESTS_NAME) $(TESTS) $(SRCS) $(LIBS) $(LDFLAGS)
	@echo "Compilacao dos testes finalizada com sucesso."

	@echo.



# Projeto principal
$(PROJ_NAME): $(OBJETOS)
	@echo.

	@echo "Compilando o projeto principal $(PROJ_NAME): ..."
	$(CC) -o $(PROJ_NAME) $(OBJETOS) $(LIBS) $(LDFLAGS)
	@echo "Compilacao finalizada com sucesso."

	@echo.



# Arquivos .c em .o
%.o : %.c
	@echo "Compilando $< ..."
	$(CC) -c $(CFLAGS) $< -o $@
# =============================================================================================== #





# ================================== DEPENDÊNCIAS DOS ARQUIVOS ================================== #
# Arquvios de teste
# para garantir que sejam recompilados quando os arquivos de cabeçalho forem modificados
../test/test_params.c: params.c



# Arquivos do projeto principal 
# para garantir que sejam recompilados quando os arquivos de cabeçalho forem modificados
main.o: main.c params.h

params.o: params.c params.h
# =============================================================================================== #





# =======================================  CLEAN & PHONY ======================================== #
# Regra para limpar os arquivos objetos e o executável
clean:
	@echo.

	@echo Limpando os arquivos de teste...
	$(RM) "../test/$(TESTS_NAME)$(EXEC)"
	@echo Arquivos de teste limpos com sucesso!

	@echo.

	@echo Limpando os arquivos objetos e o executavel...
	$(RM) $(OBJETOS) $(PROJ_NAME)$(EXEC)
	@echo Arquivos objetos e executavel limpos com sucesso!

	@echo.

# phony para evitar conflitos com arquivos chamados "clean" ou "all" no diretório
.PHONY: clean
# =============================================================================================== #