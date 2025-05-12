#include <stdio.h>
#include <string.h>
#include "auth.h"

// Kullanıcı girişi doğrulaması
int login(const char *username, const char *password) {
    FILE *file = fopen("data/users.txt", "r");
    if (file == NULL) {
        printf("\nHata: Dosya acilamadi.\n");
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char user[50], pass[50];
        sscanf(line, "%49[^,],%49s", user, pass);
        if (strcmp(user, username) == 0 && strcmp(pass, password) == 0) {
            fclose(file);
            return 1; // Başarıyla giriş yapıldı
        }
    }

    fclose(file);
    return 0; // Giriş başarısız
}




