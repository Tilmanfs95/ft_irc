/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 02:15:27 by tfriedri          #+#    #+#             */
/*   Updated: 2023/09/30 15:32:24 by tfriedri         ###   ########.fr       */
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
#include "Channel.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "defines.hpp"

class Server
{
private:
    std::string             name; // servername/hostname
    int                     port;
    int                     socket;
    struct sockaddr_in      address;
    std::vector<Channel>    channels; // maybe a map with name as key?
    std::map<int, Client>   clients; // <socket, Client> // maybe better <nickname, Client> and another <socket, nickname> ?
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
    void                    addNewClient(struct sockaddr_in address, socklen_t addrlen);
    void                    removeClient(int fds_index);
};

#endif
