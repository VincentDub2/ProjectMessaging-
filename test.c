#include <stdio.h>

int main() {
    char manual_file_path[] = "manual.txt";
    FILE *file = fopen(manual_file_path, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier manual.txt");
        return 1;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }
    fclose(file);

    return 0;
}