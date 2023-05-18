CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS =

# Fichiers sources
COMMON_SRC = $(shell find src -maxdepth 1 -name '*.c')
CLIENT_SPECIFIC_SRC = $(shell find src/client -name '*.c')
SERVEUR_SPECIFIC_SRC = $(shell find src/serveur -name '*.c')

# Fichiers objets
COMMON_OBJ = $(COMMON_SRC:.c=.o)
CLIENT_OBJ = $(CLIENT_SPECIFIC_SRC:.c=.o)
SERVEUR_OBJ = $(SERVEUR_SPECIFIC_SRC:.c=.o)

# Fichiers headers
HEADERS = $(shell find include/ -name '*.h')

# Exécutables
CLIENT_EXEC = client
SERVEUR_EXEC = serveur

all: mrproper $(CLIENT_EXEC) $(SERVEUR_EXEC)

$(CLIENT_EXEC): $(COMMON_OBJ) $(CLIENT_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(SERVEUR_EXEC): $(COMMON_OBJ) $(SERVEUR_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f $(COMMON_OBJ) $(CLIENT_OBJ) $(SERVEUR_OBJ)

mrproper: clean
	rm -f $(CLIENT_EXEC) $(SERVEUR_EXEC)

.PHONY: all clean mrproper
