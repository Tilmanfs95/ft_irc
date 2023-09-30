#include "Server.hpp"
#include "Message.hpp"

Server::Server(/* args */)
{
}

Server::Server(const char* name, const char* port, const char* password)
    : name(name), port(atoi(port)), password(password)
{
    if (this->port < 1024 || this->port > 65535)
        throw std::invalid_argument("Invalid port number");
    // Create socket
    if ((this->socket = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("Socket creation failed");
    // set server address
    memset(&this->address, 0, sizeof(this->address));
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(this->port);
    if (this->address.sin_port == 0)
        throw std::runtime_error("Invalid port number");
    // bind socket to address
    if (bind(this->socket, (struct sockaddr *)&this->address, sizeof(this->address)) < 0)
        throw std::runtime_error("Socket binding failed");
    // listen for connections
    if (listen(this->socket, MAX_CLIENTS) < 0)
        throw std::runtime_error("Socket listening failed");
    // set fds to -1
    // for (int i = 0; i < MAX_CLIENTS + 1; i++)
    for (int i = 0; i < MAX_CLIENTS + 1; i++)
        this->fds[i].fd = -1;
    // set fds[0] to server socket
    this->fds[0].fd = this->socket;
    this->fds[0].events = POLLIN;
    std::cout << "Server created" << std::endl;
}


Server::~Server()
{
}

// Setters

void                    Server::setPort(unsigned int port)
{
    this->port = port;
}

// Getters

int                     Server::getSocket() const
{
    return (this->socket);
}

unsigned int            Server::getPort() const
{
    return (this->port);
}

// Methods

void                    Server::run()
{
    struct sockaddr_in      client_address;
    socklen_t               client_address_len = sizeof(client_address);
    while (1) // TODO: set this to a bool variable and change it to false when we want to stop the server !
    {
        int client_socket;
        int numReady = poll(this->fds, MAX_CLIENTS + 1, -1);
        if (numReady < 0)
            throw std::runtime_error("Polling failed");
        
         // TODO:
         // should following if statement should be inside the for loop !!!!!
         // notice: all new connections are handled here
         // because they all go to fds[0]
        if (this->fds[0].revents & POLLIN)
        {
            try
            {
                if ((client_socket = accept(this->socket, (struct sockaddr *)&client_address, &client_address_len)) < 0)
                    throw std::runtime_error("Accepting failed");
                std::cout << "New connection from " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << std::endl;
                // add client to fds

                // Do we also should add the client to the clients vector?
                // maybe we should add an information, that the client is not yet registered

                for (int i = 1; i < MAX_CLIENTS + 1; i++)
                {
                    if (this->fds[i].fd == -1)
                    {
                        this->fds[i].fd = client_socket;
                        this->fds[i].events = POLLIN;
                        break;
                    }
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        // for (int i = 1; i < this->fds.size() + 1; i++)
        for (int i = 1; i < MAX_CLIENTS + 1; i++)
        {
            if (this->fds[i].fd != -1 && this->fds[i].revents & POLLIN)
            {
                char buffer[1024] = {0};
                ssize_t valread = recv(this->fds[i].fd, buffer, sizeof(buffer) - 1, 0); // Needs 1025 buffer because of -1 for \0 ????
                if (valread < 0)
                {
                    std::cerr << "Error reading from socket" << std::endl;
                    // close(this->fds[i].fd);                    // how to handle this situation if we could not read from socket?
                    // this->fds[i].fd = -1;
                }
                else if (valread == 0)
                {
                    std::cout << "Client disconnected" << std::endl;
                    close(this->fds[i].fd);
                    this->fds[i].fd = -1;
                }
                else
                {
                    // Following code is for testing.
                    // We should parse the message in another function and handle it there.
                    // also we have to implement partial data receiving
                    // (so store the data in a buffer and parse it when we have a full message with \r\n at the end) (or \n) ???
                    std::cout << "Received from Socket " << this->fds[i].fd << ":\n" << buffer << std::endl;

                    std::string line;
                    std::istringstream iss(buffer);
                    while (std::getline(iss, line))
                    {
                        Message msg = Message::fromString(line);
                        std::cout << "Message:" << std::endl;
                        std::cout << "    prefix: " << msg.getPrefix() << std::endl;
                        std::cout << "    command: " << msg.getCommand() << std::endl;
                        std::cout << "    params: ";
                        for (size_t i = 0; i < msg.getParams().size(); i++)
                        {
                            std::cout << msg.getParams()[i] << " ";
                        }
                        std::cout << std::endl;
                        std::cout << "    trailing: " << msg.getTrailing() << std::endl << std::endl;
                    }
                }

            }
        }
    }
}
