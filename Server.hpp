/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 02:15:27 by tfriedri          #+#    #+#             */
/*   Updated: 2023/09/30 02:48:39 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <cstdio>
#include "Channel.hpp"
#include "Client.hpp"
#include "Message.hpp"

#define MAX_CLIENTS 10
#define SERVER_NAME "ft_irc_42.de"

class Server
{
private:
    std::string             name; // servername/hostname
    int                     port;
    int                     socket;
    struct sockaddr_in      address;
    std::vector<Channel>    channels;
    std::vector<Client>     clients;
    std::string             password;
    struct pollfd           fds[MAX_CLIENTS + 1];
    
public:
    Server(/* args */);
    Server(const char* name, const char* port, const char* password);
    ~Server();

    // Setters
    void                    setPort(unsigned int port);
    
    // Getters
    int                     getSocket() const;
    unsigned int            getPort() const;

    // Methods
    void                    run();
    void                    stop();
    // void                    addNewClient(Client client);
    void                    addNewClient(struct sockaddr_in address, socklen_t addrlen);
};

#endif
