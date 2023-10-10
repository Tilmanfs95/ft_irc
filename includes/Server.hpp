/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 02:15:27 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/10 00:10:02 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "Channel.hpp"
#include "User.hpp"
#include "Message.hpp"

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
#include "defines.hpp"

class User;
class Channel;

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
    // std::map<std::string, Channel>  channels; // <name, Channel>
    // std::map<int, User>             users; // <socket, User>
    // std::map<std::string, int>      nick_to_sock; // <nickname, socket> // only registered users

    // private methods
    void                    disconnect();
    
public:
    std::map<int, User>             users; 
    std::map<std::string, Channel>  channels; // <name, Channel>
    std::map<std::string, int>      nick_to_sock; 

    Server(/* args */);
    Server(const char* name, const char* port, const char* password);
    ~Server();

    
    // Getters
    int                     getSocket() const;
    unsigned int            getPort() const;
    std::string             getPassword() const;
    // std::string             getName() const; // not needed because we can use the SERVER_NAME

    // Methods

    // starts the server loop
    void                    run();
    // sets the running bool to false
    void					stop();
	// adds a new unregistered user to the users map
    void                    addNewUser(struct sockaddr_in address, socklen_t addrlen);
    // add user to nick_to_sock map and send welcome messages
    void                    registerUser(int socket);
    // removes user from everyting and closes the socket
    void                    removeUser(int socket);
    // reads from socket and start users processInput()-method
    void                    receiveMessage(int socket);
    // handles a message from a user
    void                    handleMessage(Message &msg, User &usr);
    // returns true if the nickname is already in use
    bool                    nickUnused(const std::string &nickname);
	// usr joins/create a channel! throws an exception if the channel name is invalid
    // void                    addChannel(std::string name, std::string key);
};

extern Server *server; // NOT SURE IF WE ARE ALLOWED TO DO THIS

#endif
