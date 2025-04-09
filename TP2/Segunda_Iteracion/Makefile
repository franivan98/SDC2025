# Archivos fuente
C_SRC=procesamiento.c
ASM_SRC=float_int.asm

# Archivos objeto
C_OBJ=procesamiento.o
ASM_OBJ=float_int.o

# Librería final
LIB=libprocesamiento.so

# Carpeta destino
DEST=../../lib

# Compiladores y flags
CC=gcc
NASM=nasm
CFLAGS=-m32 -fPIC -Wall -Wextra
LDFLAGS=-shared -lm
NASMFLAGS=-f elf32

# Regla por defecto
all: $(LIB)
	@echo "✔️ Compilación finalizada: $(LIB)"
	@mkdir -p $(DEST)
	cp $(LIB) $(DEST)/

# Librería compartida
$(LIB): $(ASM_OBJ) $(C_OBJ)
	$(CC) -m32 -shared -o $(LIB) $(ASM_OBJ) $(C_OBJ) -lm

# Compilar ASM
$(ASM_OBJ): $(ASM_SRC)
	$(NASM) $(NASMFLAGS) -o $(ASM_OBJ) $(ASM_SRC)

# Compilar C
$(C_OBJ): $(C_SRC)
	$(CC) $(CFLAGS) -c -o $(C_OBJ) $(C_SRC)

# Limpieza
clean:
	rm -f $(ASM_OBJ) $(C_OBJ) $(LIB)

.PHONY: all clean

