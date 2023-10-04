/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 12:39:09 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/04 21:48:41 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/User.hpp"
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

// std::string             Server::getName() const
// {
//     return (this->name);
// }

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
                    addNewUser(c_address, c_addrlen);
                else
                    receiveMessage(this->fds[i].fd);
            }
            else if (this->fds[i].revents & POLLOUT)
            {   
                try
                {
                    std::string msg = this->users[this->fds[i].fd].getOutMessage().toString();
                    while (msg != "")
                    {
						ssize_t sent = send(this->fds[i].fd, msg.c_str(), msg.length() > OUT_BUFFER_SIZE ? OUT_BUFFER_SIZE : msg.length(), 0);
                        if (sent < 0)
							std::cout << "\033[1;31mError sending message\033[0m" << std::endl;
						else if (sent == 0)
							std::cout << "\033[1;31mNothing sent\033[0m" << std::endl;
                        else
						{
							std::cout << "\033[0;33mSent to socket " << this->fds[i].fd << ":\033[0m " << msg.substr(0, sent);
							msg.erase(0, sent);
						}
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
    disconnect();
}

void                    Server::stop()
{
    this->running = false;
}

void                    Server::disconnect()
{
    for (size_t i = 0; i < this->fds.size(); i++)
        close(this->fds[i].fd);
    std::cout << "\n\033[1;32mServer stopped successfully\033[0m\n" << std::endl;
}

void                    Server::addNewUser(struct sockaddr_in address, socklen_t addrlen)
{
    try
    {
        int c_socket;
        if ((c_socket = accept(this->socket, (struct sockaddr *)&address, &addrlen)) < 0)
            throw std::runtime_error("Failed to accept connection");
        pollfd pfd = {c_socket, POLLIN | POLLOUT, 0}; 
        this->fds.push_back(pfd);
        // std::string ip = inet_ntoa(address.sin_addr);
        this->users.insert(std::pair<int, User>(c_socket, User(c_socket, inet_ntoa(address.sin_addr))));
		std::cout << "\033[1;32mSocket " << c_socket << ": New connection from " << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << "\033[0m" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void                    Server::registerUser(int socket)
{
    this->nick_to_sock.insert(std::pair<std::string, int>(this->users[socket].getNickname(), socket));
    this->users[socket].setRegistered(true);
    
    std::cout << "User registered! TODO: send welcome stuff to user ! ( in Server::registerUser() )" << std::endl;
    
    // send welcome stuff to user
    
    //...
}

void                    Server::removeUser(int socket)
{
    // remove user from fds vector
    for (size_t i = 1; i < this->fds.size(); i++)
    {
        if (this->fds[i].fd == socket)
        {
            this->fds.erase(fds.begin() + i);
            break;
        }
    }
    if (this->users[socket].getRegistered() == true)
    {
        // TODO: remove user from channels
        //...
        //...
        // remove user from nick_to_sock map
        for (std::map<std::string, int>::iterator it = this->nick_to_sock.begin(); it != this->nick_to_sock.end(); it++)
        {
            if (it->second == socket)
            {
                this->nick_to_sock.erase(it);
                break;
            }
        }
    }
    // remove user from users map
    this->users.erase(socket);
    // close user socket
    close(socket);
    std::cout << "User disconnected and removed" << std::endl;
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
        removeUser(socket);
    else
        this->users[socket].processInput(std::string(buffer));
        
}

bool                    Server::checkUserExists(const std::string &nickname)
{
    if (this->nick_to_sock.find(nickname) == this->nick_to_sock.end())
        return false;
    // if (this->nick_to_sock[nickname] == 0)
    //     return false;
    return true;
}
