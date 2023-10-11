/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 01:59:10 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/11 00:51:43 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include <iostream>
#include <vector>
#include <queue>
#include "Message.hpp"
#include "Server.hpp"
#include "commands.hpp"
#include "defines.hpp"

// Questions:
// -Do we need a reclaim-time for the nickname? (for network problems)

// User registration:
//      CAP LS 302                              (optional) // some clients send this or another CAP command
//      PASS <Password>                         (optional) // password for the server
//      NICK <Nickname>                         (mandatory)
//      USER <username> <mode> * <real_name>    (mandatory) (if real_name contains spaces, it must be prefixed with ':')

class Server;

class User
{
private:
    // socket file descriptor
    int         socket;
    // password accepted? (PASS command)
    bool        verified; 
    // registered? (NICK and USER command received successfully)
    bool        registered;
    // mode:
    // 0 = normal user
    // 1 = maybe a bot or anything else ?
    int         mode;
    std::string nickname;
    std::string username;
    std::string realname;
    // This should be the hostname of the client
    // In our case, we use the IP address of the client
    // maybe we can make an DNS lookup to get the hostname ?
    std::string host_ip;
    // Messages that need to be sent to the client
    std::queue<Message> out_messages;
public:
    // channelnames of the channels the user is in
    // Important:
    // - NOT in uppercase !
    std::vector<std::string> channels;
    // buffer for incoming messages
    // received messages are stored here until they are complete and can be processed
    std::string in_buffer;

    // constructors and destructor

    User();
    User(int socket, std::string host_ip);
    ~User();
    
    // setters
    
    void        setRegistered(bool registered);
    void		setVerified(bool verified);
    void        setMode(int mode);
    void		setNickname(const std::string &nickname);
    void		setUsername(const std::string &username);
    void		setRealname(const std::string &realname);
    void        addChannel(const std::string &channel);
    
    // getters
    
    int			getSocket() const;
    bool		getVerified() const;
    bool		getRegistered() const;
    std::string	getNickname() const;
    std::string	getUsername() const;
    std::string	getRealname() const;
    // returns the hostname of the client
    // (in our case, the IP address)
    std::string getHostIp() const;
    // <nickname>!<username>@<host_ip>
    std::string getUserIdent() const;
    
    // Methods
    
    // Adds a message to the out_messages queue
    void		addOutMessage(const Message &msg);
    // Returns the first message from the out_messages queue
    // and removes it
    Message     getOutMessage();
};

// helper functions
void printMessage(const Message msg);

#endif