// client_pipe.c
// Client für IPC über Named Pipes (FIFOs)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define FIFO_REQUEST "/tmp/fifo_request"
#define FIFO_RESPONSE "/tmp/fifo_response"
// gleiches Verzeichnis wie beim Server

void timeout_handler(int sig) {
    // signal 14 ist SIGALRM → wenn alarm-Zeit abläuft
    printf("\n❌ Timeout! Client hat zu lange gewartet.\n");
    exit(1); // Programm beenden
}



int main() {
    int fd_req, fd_resp;
    char buffer[256];
    const char* nachricht = "Hallo vom Client!";

    // Signalhandler für Timeout registrieren
    signal(SIGALRM, timeout_handler);

    // FIFO zum Schreiben öffnen (BLOCKIERT, bis Server aufmacht)
    alarm(10); // max. 10 Sekunden auf den Server warten
    fd_req = open(FIFO_REQUEST, O_WRONLY); // FIFO öffnen → blockiert bis Server O_RDONLY öffnet
    alarm(0); // Alarm stoppen, wenn open erfolgreich war
    if (fd_req == -1) {
        perror("Client: Fehler beim Öffnen von fifo_request"); // wenn FIFO nicht da ist o.Ä.
        exit(1);
    }

    // Nachricht an den Server schreiben
    if (write(fd_req, nachricht, strlen(nachricht) + 1) == -1) { // +1 für '\0'
        perror("Client: Fehler beim Schreiben");
        close(fd_req);
        exit(1);
    }
    

    printf("Client: Nachricht gesendet: \"%s\"\n", nachricht);
    close(fd_req); // FIFO schließen → signalisiert dem Server: fertig mit Schreiben

    // FIFO zum Lesen der Antwort öffnen (BLOCKIERT bis Server schreibt)
    alarm(10); // max. 10 Sekunden warten auf Antwort vom Server
    fd_resp = open(FIFO_RESPONSE, O_RDONLY); // blockiert, wenn Server noch nicht schreibt
    alarm(0); // Alarm stoppen, wenn open erfolgreich
    if (fd_resp == -1) {
        perror("Client: Fehler beim Öffnen von fifo_response");
        exit(1);
    }

    // Antwort lesen
    alarm(10); // max. 10 Sekunden auf Antwortdaten warten
    ssize_t bytes = read(fd_resp, buffer, sizeof(buffer)); // Antwort lesen in den Puffer
    alarm(0); // Alarm wieder deaktivieren
    if (bytes <= 0) {
        perror("Client: Fehler beim Lesen");
        close(fd_resp);
        exit(1);
    }

    buffer[bytes] = '\0'; // C-String abschließen
    printf("Client: Antwort erhalten: \"%s\"\n", buffer);
    close(fd_resp); // FIFO schließen

    return 0; // alles OK
}


