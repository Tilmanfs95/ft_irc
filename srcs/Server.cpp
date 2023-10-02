/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 12:39:09 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/02 00:01:32 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Message.hpp"


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
    // set socket options
    int opt = 1;
    if (setsockopt(this->socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
        throw std::runtime_error("Socket options setting failed");
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
    if (listen(this->socket, SOMAXCONN) < 0)
        throw std::runtime_error("Socket listening failed");
    pollfd pfd = {this->socket, POLLIN, 0};
    this->fds.push_back(pfd);
    // set running to true
    this->running = true;
    std::cout << "Server created" << std::endl;
}

Server::~Server()
{
}

// Setters

void                    Server::setRunning(bool running)
{
    this->running = running;
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

std::string             Server::getPassword() const
{
    return (this->password);
}

std::string             Server::getName() const
{
    return (this->name);
}

// Methods

void                    Server::run()
{
    struct sockaddr_in      c_address;
    socklen_t               c_addrlen = sizeof(c_address);
    while (running)
    {
        if (poll(this->fds.data(), this->fds.size(), 0) < 0 && running == true) // ore use another timeout?
            throw std::runtime_error("Polling failed");
        for (size_t i = 0; i < this->fds.size(); i++)
        {   
            if (this->fds[i].revents & POLLIN)
            {
                if (i == 0)
                    addNewClient(c_address, c_addrlen);
                else
                    receiveMessage(this->fds[i].fd);
            }
            else if (this->fds[i].revents & POLLOUT)
            {   
                try
                {
                    std::string msg = this->clients[this->fds[i].fd].getOutMessage().toString();
                    while (msg != "")
                    {
                        std::cout << "Sending message: " << msg << std::endl;
                        ssize_t sent = send(this->fds[i].fd, msg.c_str(), msg.size(), 0);
                        if (sent < 0)
                            std::cout << "Error sending message" << std::endl;
                        else
                            msg.erase(0, sent);
                    }
                }
                catch(const std::exception& e)
                {
                    // std::cerr << e.what() << '\n';
                }
            }
            // else if (this->fds[i].revents & POLLERR)
            // {
            //     std::cout << "Error on socket " << this->fds[i].fd << std::endl;
            //     // what to do here?
            // }
        }
    }
    stop();
}

void                    Server::stop()
{
    // disconnect all clients
    for (size_t i = 0; i < this->fds.size(); i++)
        close(this->fds[i].fd);
    std::cout << "\n\033[1;32mServer stopped successfully\033[0m\n" << std::endl;
}

void                    Server::addNewClient(struct sockaddr_in address, socklen_t addrlen)
{
    try
    {
        int c_socket;
        if ((c_socket = accept(this->socket, (struct sockaddr *)&address, &addrlen)) < 0)
            throw std::runtime_error("Failed to accept connection");
        pollfd pfd = {c_socket, POLLIN | POLLOUT, 0}; 
        this->fds.push_back(pfd);
        this->clients.insert(std::pair<int, Client>(c_socket, Client(c_socket)));
        std::cout << "New connection from " << "\033[1;32m" << inet_ntoa(address.sin_addr) << "\033[0m" << ":" << "\033[1;32m" << ntohs(address.sin_port) << "\033[0m" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void                    Server::removeClient(int socket)
{
    // TODO: remove client from channels
    
    // remove client from fds vector
    for (size_t i = 1; i < this->fds.size(); i++)
    {
        if (this->fds[i].fd == socket)
        {
            this->fds.erase(fds.begin() + i);
            break;
        }
    }
    // remove client from clients map
    this->clients.erase(socket);
    // remove client from nick_to_sock map
    for (std::map<std::string, int>::iterator it = this->nick_to_sock.begin(); it != this->nick_to_sock.end(); it++)
    {
        if (it->second == socket)
        {
            this->nick_to_sock.erase(it);
            break;
        }
    }
    // close client socket
    close(socket);
    std::cout << "Client disconnected and removed" << std::endl;
}

void                    Server::receiveMessage(int socket)
{
    char buffer[BUFFER_SIZE] = {0};
    ssize_t valread = recv(socket, buffer, sizeof(buffer) - 1, 0);
    if (valread < 0)
    {
        // how to handle this situation if we could not read from socket?
        std::cerr << "Error reading from socket" << std::endl;
    }
    else if (valread == 0)
        removeClient(socket);
    else
        this->clients[socket].processInput(std::string(buffer));
        
}