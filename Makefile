CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS =

# Fichiers sources
CLIENT_SRC = $(wildcard src/client/*.c src/*.c)
SERVEUR_SRC = $(wildcard src/serveur/*.c src/*.c)

# Fichiers objets
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)
SERVEUR_OBJ = $(SERVEUR_SRC:.c=.o)

# Exécutables
CLIENT_EXEC = client
SERVEUR_EXEC = serveur

all: $(CLIENT_EXEC) $(SERVEUR_EXEC)

$(CLIENT_EXEC): $(CLIENT_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(SERVEUR_EXEC): $(SERVEUR_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o  $@ -c $< $(CFLAGS)

clean:
	rm -f src/client/*.o src/serveur/*.o src/*.o

mrproper: clean
	rm -f $(CLIENT_EXEC) $(SERVEUR_EXEC)

.PHONY: all clean mrproper

