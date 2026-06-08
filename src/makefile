# Makefile para o projeto TED - Trabalho de Estruturas de Dados
# Autor: André Felipe Ijiri Ribeiro

PROJ_NAME = ted
ALUNO 	  = Andre_Ijiri	# Nome do aluno (sem espaços, para uso em nomes de arquivos)
all: $(PROJ_NAME)


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
# =============================================================================================== #





# ================================= COMPILAÇÃO PROJ. PRINCIPAL ================================== #
# Arquivos-fonte e objetos
OBJETOS = main.o params.o program.o hashBin.o geo.o svg.o qry.o

main.o: main.c params.h program.h hashBin.h geo.h svg.h qry.h

params.o:  params.c params.h
program.o: program.c program.h
hashBin.o: hashBin.c hashBin.h
geo.o:     geo.c geo.h
svg.o:     svg.c svg.h
qry.o:     qry.c qry.h

# Projeto principal
$(PROJ_NAME): $(OBJETOS)
	@echo.
	@echo.
	@echo.
	@echo.

	@echo "Compilando o projeto principal $(PROJ_NAME): ..."
	$(CC) -o $(PROJ_NAME) $(OBJETOS) $(LIBS) $(LDFLAGS)
	@echo "Compilacao finalizada com sucesso."

	@echo.

# Arquivos .c em .o
%.o : %.c
	@echo.
	@echo "Compilando $< ..."
	$(CC) -c $(CFLAGS) $< -o $@
# =============================================================================================== #





# ==================================== COMPILAÇÃO DOS TESTES ==================================== #
test_params: ../test/test_params.c params.c
	@echo.
	@echo "Compilando $< ..."
	$(CC) ../test/test_params.c params.c -o ../test/test_params $(LIBS) $(LDFLAGS)
	@echo.

test_program: ../test/test_program.c program.c params.c hashBin.c geo.c svg.c
	@echo "Compilando $< ..."
	$(CC) ../test/test_program.c program.c params.c hashBin.c geo.c svg.c -o ../test/test_program $(LIBS) $(LDFLAGS)
	@echo.

test_hashBin: ../test/test_hashBin.c hashBin.c params.c
	@echo "Compilando $< ..."
	$(CC) ../test/test_hashBin.c hashBin.c params.c -o ../test/test_hashBin $(LIBS) $(LDFLAGS)
	@echo.

test_geo: ../test/test_geo.c geo.c params.c hashBin.c svg.c
	@echo "Compilando $< ..."
	$(CC) ../test/test_geo.c geo.c params.c hashBin.c svg.c -o ../test/test_geo $(LIBS) $(LDFLAGS)
	@echo.

test_svg: ../test/test_svg.c svg.c
	@echo "Compilando $< ..."
	$(CC) ../test/test_svg.c svg.c -o ../test/test_svg $(LIBS) $(LDFLAGS)
	@echo.

test_qry: ../test/test_qry.c qry.c svg.c params.c hashBin.c
	@echo "Compilando $< ..."
	$(CC) ../test/test_qry.c qry.c svg.c params.c hashBin.c -o ../test/test_qry $(LIBS) $(LDFLAGS)
	@echo.



compila_tests: test_params test_program test_hashBin test_geo test_svg test_qry
	@echo.
	@echo.
	@echo "Compilando os testes de unidade para PARAMS..."
	@echo "Compilando os testes de unidade para PROGRAM..."
	@echo "Compilando os testes de unidade para HASHBIN..."
	@echo "Compilando os testes de unidade para GEO..."
	@echo "Compilando os testes de unidade para SVG..."
	@echo "Compilando os testes de unidade para QRY..."
	@echo.
	@echo "Compilacao dos testes de unidade finalizada com sucesso."
	@echo.



ted_tests: compila_tests
	@echo.
	@echo.

	@echo "Executando os testes de unidade para os parametros..."
	../test/test_params$(EXEC)

	@echo.
	@echo.
	@echo.
	../test/test_program$(EXEC)

	@echo.
	@echo.
	@echo.
	../test/test_hashBin$(EXEC)

	@echo.
	@echo.
	@echo.
	../test/test_geo$(EXEC)

	@echo.
	@echo.
	@echo.
	../test/test_svg$(EXEC)

	@echo.
	@echo.
	@echo.
	../test/test_qry$(EXEC)

	@echo.
	@echo.
	@echo.
	@echo.
	@echo "Testes de unidade para os parametros executados com sucesso."

	@echo.
# =============================================================================================== #





# =======================================  CLEAN & PHONY ======================================== #
# Regra para limpar os arquivos objetos e o executável
clean:
	@echo.

	@echo Limpando os arquivos de teste...
	$(RM) "../test/test_params$(EXEC)"
	$(RM) "../test/test_program$(EXEC)"
	$(RM) "../test/test_hashBin$(EXEC)"
	$(RM) "../test/test_geo$(EXEC)"
	$(RM) "../test/test_svg$(EXEC)"
	$(RM) "../test/test_qry$(EXEC)"
	$(RM) "./t1.svg"
	@echo Arquivos de teste limpos com sucesso!

	@echo.

	@echo Limpando os arquivos objetos e o executavel...
	$(RM) $(OBJETOS) $(PROJ_NAME)$(EXEC)
	@echo Arquivos objetos e executavel limpos com sucesso!

	@echo.

.PHONY: all
.PHONY: clean
.PHONY: ted_tests 
.PHONY: test_params test_program test_hashBin test_geo test_svg test_qry
.PHONY: compila_tests
# =============================================================================================== #