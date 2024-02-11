#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8765
#define NMIN 4
#define NMAX 10
#define DISP 1
#define NON_DISP 0

char buffer[256];
char answer[256];
int n = 6;
int nLibri;
int disp = 1;

struct Libro {
    int idLibro;
    char Titolo[256];
    char Autore[256];
    int disponibile;
};

void handler (int signo) {
    if (disp == 1) { disp = 0; }
    else { disp = 1; }
    printf("Messaggio ricevuto\n");
}

void get_lista_disponibili (int msgsock);

void prenota_libro (int msgsock, int idLibro);

void restituissci_libro (int msgsock, int codice);

// una pipe per ogni libro, la seconda parte è per indicare 0 per la lettura e 1 per la scrittura

int piped[10][2];

int main(int argc, char* argv[]) {
    struct sockaddr_in server;
    int sock, msgsock, len;
    char msg, titolo[256], autore[256];
    struct Libro libri;

    struct sigaction sig;
    sigset_t sigmask, zeromask;

    sig.sa_flags = SA_RESTART;
    sig.sa_handler = handler;
    sigemptyset(&sig.sa_mask);

    sigaction(SIGUSR1, &sig, NULL);




    /*
    if (argc > 2) {
        sprintf(stderr, "Errore nel numero di argomenti");
        exit(1);
    } else if (argc == 2) {
        // trasformo argv[1] in un intero
        n = atoi(argv[1]);

        if (n < NMIN || n > NMAX) {
            sprintf(stderr, "Errore nel numero scelto");
            exit(1);
        }
    }
     */

    nLibri = n;

    for (int i = 0; i < n; i++) {
        if (pipe(piped[i]) < 0) {
            perror("creating pipe");
            exit(9);
        }
    }

    for (int i = 0; i < n; i++) {
        libri.idLibro = i;
        sprintf(titolo, "Titolo%d", i);
        strcpy(libri.Titolo, titolo);
        sprintf(autore, "Autore%d", i);
        strcpy(libri.Autore, autore);
        libri.disponibile = DISP;

        write(piped[i][1], &libri, sizeof libri);
    }


    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    printf("%d\n", getpid());

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("sock error");
        exit(2);
    }

    if ((bind(sock, (struct sockaddr*) &server, sizeof server)) < 0) {
        perror("binding error");
        exit(3);
    }

    len = sizeof server;

    if ((getsockname(sock, (struct sockaddr*)&server, &len)) < 0) {
        perror("getting sock name error");
        exit(4);
    }

    listen(sock, 5);

    do {
        msgsock = accept(sock, (struct sockaddr*)&server, &len);

        if (msgsock == -1) {
            perror("accept");
            exit(4);
        }

        if (fork() == 0) {


            close(sock);
            int id_libro;

            if (read(msgsock, buffer, sizeof buffer) < 0) {
                perror("reading error");
                exit(7);
            }

            if (sscanf(buffer, "%s%d", &msg, &id_libro) > 2) {
                perror("errore nella lettura");
                exit(8);
            }
            printf("Messaggio: %s ID_LIBRO: %d\n", &msg, id_libro);

            switch (msg) {
                case 'L':
                    get_lista_disponibili(msgsock);
                    break;
                case 'P':
                    prenota_libro(msgsock, id_libro);
                    break;
                case 'R':
                    restituissci_libro(msgsock, id_libro);
                    break;
                case 'F':
                    sprintf(buffer, "FINE...\n");
                    write(msgsock, buffer, strlen(buffer) + 1);
            }

            close(msgsock);
            exit(0);
        }
        else {
            close(msgsock);
        }
    } while (1);
}

void get_lista_disponibili (int msgsock) {
    struct Libro libri;

    char buffer [256];

    for (int i = 0; i < nLibri; i++) {
        read(piped[i][0], &libri, sizeof(libri));
        if (libri.disponibile) {
            sprintf(buffer, "Autore: %s Titolo: %s\n", libri.Autore, libri.Titolo);
            write(msgsock, buffer, strlen(buffer) + 1);
        }
    }
}

void prenota_libro (int msgsock, int idLibro) {
    struct Libro libri;
    char buffer[256];
    read(piped[idLibro][0], &libri, sizeof(libri));

    if (libri.disponibile == DISP) {
        sprintf(buffer, "OK CODICE %d", libri.idLibro);
        write(msgsock, buffer, strlen (buffer) + 1);
        libri.disponibile = NON_DISP;
    }
    else {
        sprintf(buffer, "NON DISPONIBILE\n");
        write(msgsock, buffer, strlen (buffer) + 1);
    }
    write(piped[idLibro][1], &libri, sizeof(libri));
}

void restituissci_libro (int msgsock, int codice) {
    struct Libro libri;
    char buffer[256];

    read(piped[codice][0], &libri, sizeof(libri));

    if (libri.disponibile == DISP) {
        sprintf(buffer, "Libro non prelevato\n");
        write(msgsock, buffer, strlen(buffer) + 1);
    }
    else {
        sprintf(buffer, "Libro restituito, CODICE: %d", codice);
        write(msgsock, buffer, strlen(buffer) + 1);
        libri.disponibile = DISP;
    }

    write(piped[codice][1], &libri, sizeof(libri));
}