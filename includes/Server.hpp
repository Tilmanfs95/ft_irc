/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 02:15:27 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/01 18:39:29 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <cstdio>
#include <signal.h>
#include "Channel.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "defines.hpp"

class Client;

class Server
{
private:
    bool                            running;
    std::string                     name; // servername/hostname
    int                             port;
    std::string                     password;
    int                             socket;
    struct sockaddr_in              address;
    std::vector<struct pollfd>      fds;
    std::map<std::string, Channel>  channels; // <name, Channel>
    std::map<int, Client>           clients; // <socket, Client>
    std::map<std::string, int>      nick_to_sock; // <nickname, socket>

public:
    Server(/* args */);
    Server(const char* name, const char* port, const char* password);
    ~Server();

    // Setters
    void                    setRunning(bool running);
    
    // Getters
    int                     getSocket() const;
    unsigned int            getPort() const;
    std::string             getPassword() const;
    std::string             getName() const;

    // Methods
    void                    run();
    void                    stop();
    void                    addNewClient(struct sockaddr_in address, socklen_t addrlen);
    void                    removeClient(int socket);
    void                    receiveMessage(int socket);
};

extern Server *server; // NOT SURE IF WE ARE ALLOWED TO DO THIS

#endif
