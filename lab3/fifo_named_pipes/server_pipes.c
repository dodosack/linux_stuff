// server_pipe.c
// Server für IPC über Named Pipes (FIFOs)
// bidirektional

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>

#define FIFO_REQUEST "/tmp/fifo_request"
#define FIFO_RESPONSE "/tmp/fifo_response"
// hier datei verzeichnis

/*void cleanup_and_exit_succes(int sig) // sig wichtig weil signal(SIGINT, cleanup_and_exit) so cleanup_and_exit(2);   // 2 = SIGINT auffruft
// am besten 2 machen eine für fehler und eine für succes
{ // signal 2 ist ctrl+c

    unlink(FIFO_REQUEST); // löscht die datein damit nicht übrig bleibt
    unlink(FIFO_RESPONSE);
    printf("\nServer beendet. FIFOs gelöscht.\n");

    exit(0); // Beenden des Programms
}
void cleanup_and_exit_failure(int sig) // sig wichtig weil signal(SIGINT, cleanup_and_exit) so cleanup_and_exit(2);   // 2 = SIGINT auffruft
// am besten 2 machen eine für fehler und eine für succes
{ // signal 2 ist ctrl+c

    unlink(FIFO_REQUEST); // löscht die datein damit nicht übrig bleibt
    unlink(FIFO_RESPONSE);
    printf("\nServer beendet. FIFOs gelöscht.\n");

    exit(1); // Beenden des Programms
}*/
void cleanup()
{
    unlink(FIFO_REQUEST);
    unlink(FIFO_RESPONSE);
    printf("\nFIFOs gelöscht.\n");
}

void cleanup_and_exit_success(int sig)
{
    cleanup();
    printf("Beendet mit Erfolg.\n");
    exit(0);
}

void cleanup_and_exit_failure(int sig)
{
    cleanup();
    printf("Beendet mit Fehler.\n");
    exit(1);
}

void timeout_handler(int sig)
{
    printf("\n❌ Timeout! Server hat zu lange gewartet.\n");
    cleanup_and_exit_failure(sig); // FIFO löschen und beenden
}

int main()
{
    int fd_req, fd_resp;
    char buffer[256];

    // Signalhandler registrieren für sauberes Beenden
    signal(SIGINT, cleanup_and_exit_success);
    signal(SIGTERM, cleanup_and_exit_failure);

    // signit == signalinterrupt ist per default 2
    // signal(int signalnummer, void (*handler)(int));
    signal(SIGALRM, timeout_handler); // Timeout-Handler registrieren

    // FIFOs erstellen, falls sie nicht existieren
    // prüfung ob die datei existiert
    if (access(FIFO_REQUEST, F_OK) == -1)
    {
        // access gibt 0 zurück wenn die datei existiert also prüft ob eine datei existiert
        // F_ok ist ein makro das  prüft ob etwas existiert gibt auch X_OK wenn ausführbar ist

        if (mkfifo(FIFO_REQUEST, 0666) == -1)
        {
            // int mkfifo(const char *pathname, mode_t mode);
            // macht eine fifo datei die rw rw rw
            perror("mkfifo fifo_request");
            cleanup_and_exit_failure(666);
            // exit(1);
        }
    }

    if (access(FIFO_RESPONSE, F_OK) == -1)
    {
        if (mkfifo(FIFO_RESPONSE, 0666) == -1)
        {
            perror("mkfifo fifo_response");
            cleanup_and_exit_failure(999);
        }
    }

    // wenn nix passiert dann ist die fifo da und kann benutzt werden

    printf("bis jz ist alles ok\n");
    printf("Server gestartet. Warte auf Client-Nachricht...\n");

    // FIFO zum Lesen öffnen (BLOCKIERT bis Client schreibt)
    // Öffnest du eine FIFO mit O_RDONLY und ohne das Flag O_NONBLOCK,
    // blockiert der open()-Aufruf so lange, bis mindestens ein anderer Prozess die selbe FIFO zum Schreiben (O_WRONLY) geöffnet hat

    fd_req = open(FIFO_REQUEST, O_RDONLY); // int open(const char *pathname, int flags); bekommen einen file descriptor zurück also eine zahl von 0 bis 3 -1 bei keinen zugriff
    // O_RDONLY = nur lesen
    // O_WRONLY = nur schreiben
    if (fd_req == -1)
    {
        perror("Fehler beim Öffnen von fifo_request"); // Fehler beim Öffnen von fifo_request: No such file or directory

        cleanup_and_exit_failure(0);
    }
    // blockiert dieser Befehl, bis ein anderer Prozess die FIFO zum Schreiben (O_WRONLY) öffnet.

    // Nachricht lesen

    alarm(10);                                                 // Timeout auf 10 Sekunden setzen
    ssize_t bytes_read = read(fd_req, buffer, sizeof(buffer)-1); // read mach ohne nullterminator
    alarm(0);                                                  // Timeout zurücksetzen
    // read liest die pipe und bekommt file descriptor und buffer  wo reinkopiert wird und die max größe des  buffers
    if (bytes_read <= 0) // ssize ist größe in bytes oder -1 bei fehler
    {
        perror("Fehler beim Lesen");
        close(fd_req); // schließt den file descriptor wichtig
        cleanup_and_exit_failure(0);
    }

    buffer[bytes_read-1] = '\0'; // String korrekt abschließen in c
    printf("Server: Nachricht erhalten: \"%s\"\n", buffer);
    close(fd_req); // wieder shließen

    // ferittg mit request

    // Antwort senden
    const char *antwort = "Hallo vom Server!";
    fd_resp = open(FIFO_RESPONSE, O_WRONLY); // wartet wieder auf den client
    // FIFO zum Schreiben öffnen (BLOCKIERT, bis Client liest)
    if (fd_resp == -1)
    {
        perror("Fehler beim Öffnen von fifo_response");
        cleanup_and_exit_failure(0);
    }
    alarm(10);                                    // Timeout auf 10 Sekunden setzen
    write(fd_resp, antwort, strlen(antwort) + 1); // könnte man dynamisch machen aber hier ist es egal und +1 wegen /0
    alarm(0);                                     // Timeout zurücksetzen
    printf("Server: Antwort gesendet.\n");
    close(fd_resp);

    // freiwillig
    if (close(fd_resp) == -1)
    {
        perror("Fehler beim Schließen von fifo_response");
        cleanup_and_exit_failure(1);    
    }

    cleanup_and_exit_success(0);
    return 0;
}
