#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "auth.h"
#include <conio.h>  // getch için


// E-Posta Uygulaması Logo
void print_ascii_mail_logo() {
    printf("\033[1;33m      __________________________\n");
    printf("     /                          \\\n");
    printf("    /     ________________       \\\n");
    printf("   /     /                \\       \\\n");
    printf("  |     |  E-POSTA UYGULAMASI |      |\n");
    printf("   \\     \\________________/       /\n");
    printf("    \\                          /\n");
    printf("     \\________________________/\n\n\033[0m");
}

// Ana Menü
void show_main_menu() {
    print_ascii_mail_logo(); // Logo ekleniyor

    printf("\033[1;36m    ===============================\033[0m\n");
    printf("\033[1;36m    E-POSTA UYGULAMASINA HOSGELDINIZ\n\033[0m");
    printf("\033[1;36m    ===============================\033[0m\n");
    
    printf("\033[1;33m    1 - Giris yap\033[0m\n");
    printf("\033[1;33m    2 - Kayit ol\033[0m\n");
    printf("\033[1;33m    0 - Cikis\033[0m\n");
    
    printf("\033[1;36mSeciminiz: \033[0m");
}

// E-posta işlemleri menüsü
void show_email_operations_menu() {
    printf("\033[1;33m\n    ================================\033[0m\n");
    printf("\033[1;33m    E-POSTA ISLEMLERI\033[0m\n");
    printf("\033[1;33m    ================================\033[0m\n");
    
    printf("\033[1;33m    1 - E-posta gonder\033[0m\n");
    printf("\033[1;33m    2 - E-postalarimi listele\033[0m\n");
    printf("\033[1;33m    3 - E-posta sil\033[0m\n");
    printf("\033[1;33m    0 - Cikis\033[0m\n");

    printf("\033[1;36mSeciminiz: \033[0m");
}

void get_password(char *password, int max_length) {
    int i = 0;
    char ch;

    while ((ch = getch()) != '\r' && i < max_length - 1) {  // Enter tuşuna kadar
        if (ch == '\b') {  // Backspace tuşu
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
}

// Tarih ve saati almak için fonksiyon
void get_current_datetime(char *datetime) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    sprintf(datetime, "%04d-%02d-%02d %02d:%02d:%02d", 
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, 
            tm.tm_hour, tm.tm_min, tm.tm_sec);
}

// Kullanıcıyı kaydet
void register_user() {
    char new_username[50];
    char new_password[50];

    printf("\nKullanici adi: ");
    scanf("%49s", new_username);

    printf("Sifre: ");
get_password(new_password, 50);


    FILE *file = fopen("data/users.txt", "a");
    if (file == NULL) {
        printf("\nHata: Dosya acilamadi.\n");
        return;
    }

    fprintf(file, "%s,%s\n", new_username, new_password);
    fclose(file);

    printf("\nKayit basarili! Lutfen giris yapiniz.\n");
}

// E-posta gönderme fonksiyonu
void send_email(const char *username) {
    char recipient[50], subject[100], message[500];
    char datetime[20];

    get_current_datetime(datetime);  // Tarih ve saat alınıyor

    printf("\nE-posta alicisini girin: ");
    scanf("%49s", recipient);

    printf("Konu: ");
    scanf(" %[^\n]", subject);

    printf("Mesaj: ");
    scanf(" %[^\n]", message);

    FILE *file = fopen("data/mails.txt", "a");
    if (file == NULL) {
        printf("\nHata: Dosya acilamadi.\n");
        return;
    }

    fprintf(file, "From: %s\nTo: %s\nSubject: %s\nDate: %s\nMessage: %s\n\n", 
            username, recipient, subject, datetime, message);
    fclose(file);

    printf("\nE-posta basariyla gonderildi!\n");
}

// E-postaları listeleme fonksiyonu (kişisel gelen/giden ayrımı ile)
void list_emails(const char *username) {
    char line[500];
    FILE *file = fopen("data/mails.txt", "r");
    if (file == NULL) {
        printf("\nHata: Dosya acilamadi.\n");
        return;
    }

    char from[50] = "", to[50] = "", subject[100] = "", datetime[20] = "", message[500] = "";
    typedef struct {
        char from[50];
        char subject[100];
        char message[500];
    } Email;

    Email receivedMails[100];
    int receivedCount = 0;
    int hasReceived = 0, hasSent = 0;

    // === GELEN E-POSTALAR ===
    printf("\n===============\nGELEN E-POSTALAR:\n===============\n");
    rewind(file);
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "From: ", 6) == 0) sscanf(line, "From: %s", from);
        else if (strncmp(line, "To: ", 4) == 0) sscanf(line, "To: %s", to);
        else if (strncmp(line, "Subject: ", 9) == 0) sscanf(line, "Subject: %[^\n]", subject);
        else if (strncmp(line, "Date: ", 6) == 0) sscanf(line, "Date: %[^\n]", datetime);
        else if (strncmp(line, "Message: ", 9) == 0) sscanf(line, "Message: %[^\n]", message);
        else if (strcmp(line, "\n") == 0) {
            if (strcmp(to, username) == 0) {
                printf("- Konu: %s\n  Kimden: %s\n  Tarih: %s\n  Mesaj: %s\n-------------------\n",
                       subject, from, datetime, message);
                strcpy(receivedMails[receivedCount].from, from);
                strcpy(receivedMails[receivedCount].subject, subject);
                strcpy(receivedMails[receivedCount].message, message);
                receivedCount++;
                hasReceived = 1;
            }
        }
    }
    if (!hasReceived)
        printf("Gelen e-posta bulunamadi.\n");

    // === GIDEN E-POSTALAR ===
    printf("\n===============\nGIDEN E-POSTALAR:\n===============\n");
    rewind(file);
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "From: ", 6) == 0) sscanf(line, "From: %s", from);
        else if (strncmp(line, "To: ", 4) == 0) sscanf(line, "To: %s", to);
        else if (strncmp(line, "Subject: ", 9) == 0) sscanf(line, "Subject: %[^\n]", subject);
        else if (strncmp(line, "Date: ", 6) == 0) sscanf(line, "Date: %[^\n]", datetime);
        else if (strncmp(line, "Message: ", 9) == 0) sscanf(line, "Message: %[^\n]", message);
        else if (strcmp(line, "\n") == 0) {
            if (strcmp(from, username) == 0) {
                printf("Kime: %s\nKonu: %s\nTarih: %s\nMesaj: %s\n-------------------\n",
                       to, subject, datetime, message);
                hasSent = 1;
            }
        }
    }
    if (!hasSent)
        printf("Giden e-posta bulunamadi.\n");

    // === YANIT SORUSU ===
    if (hasReceived) {
        int yanitSecim;
        printf("\nBir e-postayi yanitlamak istiyor musunuz? (1 = Evet, 0 = Hayir): ");
        scanf("%d", &yanitSecim);

        if (yanitSecim == 1) {
            char konuSecimi[100];
            int bulundu = 0;
            printf("Hangi konuya ait e-postayi yanitlamak istiyorsunuz? (Konu basligini yaziniz): ");
            getchar(); // \n temizle
            fgets(konuSecimi, sizeof(konuSecimi), stdin);
            konuSecimi[strcspn(konuSecimi, "\n")] = '\0';

            for (int i = 0; i < receivedCount; i++) {
                if (strcmp(receivedMails[i].subject, konuSecimi) == 0) {
                    char yanit[500];
                    char currentDate[20];
                    get_current_datetime(currentDate);

                    printf("Yanitiniz: ");
                    fgets(yanit, sizeof(yanit), stdin);
                    yanit[strcspn(yanit, "\n")] = '\0';

                    FILE *mailFile = fopen("data/mails.txt", "a");
                    if (mailFile != NULL) {
                        fprintf(mailFile, "From: %s\nTo: %s\nSubject: YANIT: %s\nDate: %s\nMessage: %s\n\n",
                                username,
                                receivedMails[i].from,
                                receivedMails[i].subject,
                                currentDate,
                                yanit);
                        fclose(mailFile);
                        printf("Yanit basariyla gonderildi.\n");
                    } else {
                        printf("Yanit gonderilemedi.\n");
                    }
                    bulundu = 1;
                    break;
                }
            }

            if (!bulundu)
                printf("Belirtilen konuya ait e-posta bulunamadı.\n");
        }
    }

    fclose(file);
}


// E-posta silme fonksiyonu
void delete_email(const char *username) {
    FILE *file = fopen("data/mails.txt", "r");
    if (file == NULL) {
        printf("\nHata: Dosya acilamadi.\n");
        return;
    }

    char line[500];
    FILE *tempFile = fopen("data/tempmails.txt", "w");
    if (tempFile == NULL) {
        printf("\nHata: Geçici dosya acilamadi.\n");
        fclose(file);
        return;
    }

    char from[50] = "";
    char to[50] = "";
    char subject[100] = "";
    char message[500] = "";
    int emailFound = 0;

    printf("\nHangi e-posta silinsin? (Kimden): ");
    char sender[50];
    scanf("%49s", sender);

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "From: ", 6) == 0)
            sscanf(line, "From: %s", from);
        else if (strncmp(line, "To: ", 4) == 0)
            sscanf(line, "To: %s", to);
        else if (strncmp(line, "Subject: ", 9) == 0)
            sscanf(line, "Subject: %[^\n]", subject);
        else if (strncmp(line, "Message: ", 9) == 0)
            sscanf(line, "Message: %[^\n]", message);
        else if (strcmp(line, "\n") == 0) {
            if (strcmp(from, sender) == 0 && strcmp(to, username) == 0) {
                emailFound = 1;
            } else {
                fprintf(tempFile, "From: %s\nTo: %s\nSubject: %s\nMessage: %s\n\n", from, to, subject, message);
            }
        }
    }

    fclose(file);
    fclose(tempFile);

    if (emailFound) {
        remove("data/mails.txt");
        rename("data/tempmails.txt", "data/mails.txt");
        printf("\nE-posta silindi.\n");
    } else {
        printf("\nE-posta bulunamadi.\n");
    }
}

int main() {
    int secim;
    char username[50];
    char password[50];

    while (1) {
        show_main_menu();
        scanf("%d", &secim);

        switch (secim) {
            case 1:
                printf("\nKullanici adi: ");
                scanf("%49s", username);

                printf("Sifre: ");
get_password(password, 50);


                if (login(username, password)) {
                    printf("\nGiris basarili! Hosgeldin %s\n\n", username);

                    int email_secim;
                    while (1) {
                        show_email_operations_menu();
                        scanf("%d", &email_secim);

                        switch (email_secim) {
                            case 1:
                                send_email(username);
                                break;

                            case 2:
                                list_emails(username);
                                break;

                            case 3:
                                delete_email(username);
                                break;

                            case 0:
                                printf("\nHesabinizdan cikiliyor...\n");
                                break;  // Burada sadece döngüden çıkıyoruz

                            default:
                                printf("\nGecersiz secim. Lutfen tekrar deneyin.\n\n");
                                break;
                        }

                        if (email_secim == 0) {
                            break;  // Ana menüye dönüyoruz
                        }
                    }

                } else {
                    printf("\nGiris basarisiz. Lutfen tekrar deneyin.\n\n");
                }
                break;

            case 2:
                register_user();
                break;

            case 0:
                printf("\nProgramdan cikiliyor...\n");
                exit(0);

            default:
                printf("\nGecersiz secim. Lutfen tekrar deneyin.\n\n");
                break;
        }
    }

    return 0;
}














