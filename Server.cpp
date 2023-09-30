/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 12:39:09 by tfriedri          #+#    #+#             */
/*   Updated: 2023/09/30 15:27:43 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
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
    struct sockaddr_in      c_address;
    socklen_t               c_addrlen = sizeof(c_address);
    while (1) // TODO: set this to a bool variable and change it to false when we want to stop the server !
    {
        if (poll(this->fds, MAX_CLIENTS + 1, -1) < 0)
            throw std::runtime_error("Polling failed");
        for (int i = 0; i < MAX_CLIENTS + 1; i++)
        {   
            if (this->fds[i].revents & POLLIN)
            {
                if (i == 0) // add new client
                        addNewClient(c_address, c_addrlen);
                else // read from client
                {
                    char buffer[BUFFER_SIZE] = {0};
                    ssize_t valread = recv(this->fds[i].fd, buffer, sizeof(buffer) - 1, 0); // Do we need 1025 buffer because of -1 for \0 ????
                    if (valread < 0)
                    {
                        // how to handle this situation if we could not read from socket?
                        
                        std::cerr << "Error reading from socket" << std::endl;
                        // close(this->fds[i].fd);                    
                        // this->fds[i].fd = -1;
                    }
                    else if (valread == 0)
                        removeClient(i);
                    else
                        this->clients[this->fds[i].fd].processInput(buffer);
                }

            }
        }
    }
}

void                    Server::stop()
{
    ///TODO: implement this
}

void                    Server::addNewClient(struct sockaddr_in address, socklen_t addrlen)
{
    try
    {
        int c_socket;
        if ((c_socket = accept(this->socket, (struct sockaddr *)&address, &addrlen)) < 0)
            throw std::runtime_error("Failed to accept connection");
        for (int i = 1; i < MAX_CLIENTS + 1; i++)
        {
            if (this->fds[i].fd == -1)
            {
                this->fds[i].fd = c_socket;
                this->fds[i].events = POLLIN;
                break;
            }
        }
        this->clients[c_socket] = Client(c_socket);
        std::cout << "New connection from " << "\033[1;32m" << inet_ntoa(address.sin_addr) << "\033[0m" << ":" << "\033[1;32m" << ntohs(address.sin_port) << "\033[0m" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void                    Server::removeClient(int fds_index)
{
    int socket = this->fds[fds_index].fd;
    this->fds[fds_index].fd = -1;
    this->clients.erase(socket);
    close(socket);
    std::cout << "Client disconnected and removed" << std::endl;
}
