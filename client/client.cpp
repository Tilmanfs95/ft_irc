#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <cstdio>
#include <netdb.h>
#include <readline/readline.h>
#include <readline/history.h>


void receiveMessages(int clientSocket) {
    char buffer[1024];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

        if (bytesRead <= 0) {
            perror("Fehler beim Empfangen der Nachricht vom Server");
            break;
        }

        // Farbige Ausgabe für den vom Server empfangenen Text
        std::cout << "\033[1;31m" << buffer << "\033[0m" << std::flush;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Verwendung: " << argv[0] << " <Host>:<Port>" << std::endl;
        return 1;
    }

    // Das Argument in Host und Port aufteilen
    char* arg = argv[1];
    char* hostStr = strtok(arg, ":");
    char* portStr = strtok(NULL, ":");

    if (hostStr == NULL || portStr == NULL) {
        std::cerr << "Ungültiges Argumentformat. Verwenden Sie <Host>:<Port>" << std::endl;
        return 1;
    }

    int port = std::atoi(portStr);

    // Hostnamen in IP-Adresse auflösen
    struct addrinfo hints;
    struct addrinfo* result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostStr, NULL, &hints, &result) != 0) {
        perror("Fehler beim Auflösen des Hostnamens");
        return 1;
    }

    const char* ip = inet_ntoa(((struct sockaddr_in*)(result->ai_addr))->sin_addr);

    freeaddrinfo(result);

    // Socket erstellen
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Fehler beim Erstellen des Sockets");
        return 1;
    }

    // Serveradresse einrichten
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &(serverAddress.sin_addr)) <= 0) {
        perror("Ungültige IP-Adresse");
        return 1;
    }

    // Verbindung zum Server herstellen
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Fehler beim Herstellen der Verbindung");
        return 1;
    }
    std::cout << std::endl << "\033[1;32m" << "Verbunden mit " << hostStr << ":" << port << "\033[0m" << std::endl << std::endl;

    // Thread für den Nachrichtenempfang starten
    std::thread receiveThread(receiveMessages, clientSocket);

    // Nachrichten senden
    while (1) {
        char* userInput = readline("> ");
        if (userInput == nullptr) {
            break; // Eingabe beenden, wenn EOF erreicht wird (z.B. durch Strg+D)
        }
        if (strlen(userInput) > 0) {
            add_history(userInput); // Zur Historie hinzufügen
            strcat(userInput, "\n"); // Fügt einen Zeilenumbruch hinzu
            send(clientSocket, userInput, strlen(userInput), 0);
        }
        free(userInput);
    }

    // Warten, bis der Empfangsthread beendet ist
    receiveThread.join();

    // Verbindung schließen
    close(clientSocket);

    return 0;
}
