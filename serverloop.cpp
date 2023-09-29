#include "Server.hpp"


int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddrLen = sizeof(clientAddress);
    std::vector<struct pollfd> fds(MAX_CLIENTS + 1); // +1 for server socket
	// set all fds to -1
	for (size_t i = 0; i < fds.size(); ++i)
		fds[i].fd = -1;

    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Error creating server socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // Bind server socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Error binding server socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) < 0) {
        perror("Error listening on server socket");
        exit(EXIT_FAILURE);
    }

    fds[0].fd = serverSocket;
    fds[0].events = POLLIN;

    std::cout << "Server listening on port " << PORT << std::endl;

    while (1) {
        int numReady = poll(&fds[0], fds.size(), -1);

        if (numReady < 0) {
            perror("Error in poll");
            exit(EXIT_FAILURE);
        }

        if (fds[0].revents & POLLIN) {
            // New client connection
            clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddrLen);
            if (clientSocket < 0) {
                perror("Error accepting client connection");
                continue;
            }
            std::cout << "New client connected: " << inet_ntoa(clientAddress.sin_addr) << " | Socket: " << clientSocket << std::endl;

            // Add client socket to poll set
            for (size_t i = 1; i < fds.size(); ++i) {
                if (fds[i].fd == -1) {
                    fds[i].fd = clientSocket;
                    fds[i].events = POLLIN;
                    break;
                }
            }
        }

        // Check for data from clients
        for (size_t i = 1; i < fds.size(); ++i) {
			// std::cout << ".";
            if (fds[i].fd != -1 && (fds[i].revents & POLLIN)) {
				// std::cout << "fds[i].fd: " << fds[i].fd << std::endl;
                char buffer[1024];
                memset(buffer, 0, sizeof(buffer));
                ssize_t bytesRead = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
                if (bytesRead <= 0) {
                    if (bytesRead < 0) {
                        perror("Error receiving data from client");
                    }
                    close(fds[i].fd);
                    std::cout << "Socket " << fds[i].fd << " disconnected" << std::endl;
                    fds[i].fd = -1;
                } else {
                    std::cout << "Received from Socket "<< fds[i].fd << ":\n" << buffer << std::endl;
                    // Echo back to the client
                    // send(fds[i].fd, buffer, bytesRead, 0);
					// send to all clients
					for (size_t j = 1; j < fds.size(); ++j) {
						if (fds[j].fd != -1 && fds[j].fd != fds[i].fd) {
							send(fds[j].fd, buffer, bytesRead, 0);
						}
					}
                }
            }
        }
    }

    close(serverSocket);

    return 0;
}
