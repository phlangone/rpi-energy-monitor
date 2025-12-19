# Compilador
CC = gcc

# Nome do binário
TARGET = monitor

# Arquivos fonte
SRC = main.c meter.c mqtt.c

# Flags de compilação
CFLAGS = -Wall -Wextra -O2

# Libmodbus
LIBS = -lmodbus -lmosquitto  -lm

# Includes
INCLUDES = -I/usr/include/modbus -I.

# Regra padrão
all: $(TARGET)

# Linkagem
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) -o $(TARGET) $(LIBS)

# Limpeza
clean:
	rm -f $(TARGET)

.PHONY: all clean
