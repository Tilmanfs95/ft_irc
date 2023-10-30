/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 02:15:27 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/30 12:57:48 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "Channel.hpp"
#include "User.hpp"
#include "Message.hpp"
#include "defines.hpp"

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

// forward declaration
class User;
class Channel;


class Server
{
private:
	// private constructor & destructor for singleton
    Server();
    ~Server();
    bool                            running;
    static std::string              name;
    static int                      port;
    static std::string              password;
    int                             socket;
    struct sockaddr_in              address;
    std::vector<struct pollfd>      fds;
    
    // closes all sockets
    // runs after the server loop is stopped
    void                    disconnect();
    // handles a message from a user
    void                    handleMessage(Message &msg, User &usr);
    
public:
    // singleton
    static Server                   *instance;
    static void                     setup(const char* name, const char* port, const char* password);
    static Server&                  getInstance();
    // A map that contains all users that are connected to the server
    // <socket, User>
    std::map<int, User>             users;
    // A map that contains all channels that are active
    // The map key is the channel name in uppercase (for case insensitive comparison)
    // <channelname in uppercase, Channel>
    std::map<std::string, Channel>  channels;
    // A map that contains all nicknames/registerd users
    // The map key is the nickname in uppercase (for case insensitive comparison)
    // <nickname in uppercase, socket>
    std::map<std::string, int>      nick_to_sock;
	// returns the password
    std::string             getPassword() const;
    // starts the server loop
    void                    run();
    // sets the running bool to false to stop the server loop
    // called from the signal handler
    void					stop();
	// adds a new unregistered user to the users map
    void                    addNewUser(struct sockaddr_in address, socklen_t addrlen);
    // add user to nick_to_sock map and send welcome messages
    // should only be called after the user has registered (NICK and USER successfully received)
    void                    registerUser(int socket);
    // removes user and closes socket
    // calls the channels removeUser() method for every channel the user is in
    void                    removeUser(int socket);
    // reads from socket and starts the handleMessage() for every complete message
    void                    receiveMessage(int socket);
};

#endif
