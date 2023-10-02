/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 01:59:10 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/02 15:01:52 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <vector>
#include <queue>
#include "Message.hpp"
#include "Server.hpp"
#include "msg_handler.hpp"
#include "defines.hpp"

// Questions:
// -Do we need the password? I don't think so.
// -Do we need a reclaim-time for the nickname? (for network problems)

// User registration:
//      CAP LS 302                              (optional) // some clients send this or another CAP command
//      PASS <Password>                         (optional) // password for the server
//      NICK <Nickname>                         (mandatory)
//      USER <username> <mode> * <real_name>    (mandatory) (if real_name contains spaces, it must be prefixed with ':')

class Server;

class Client
{
    // Important:
    // -The client must send a PASS command before sending the NICK and USER commands.
    // -The client must send a NICK and USER command before any others.
private:
    int         socket;
    // password accepted? (PASS command)
    bool        verified; 
    // registered? (NICK and USER command)
    bool        registered;
    // mode:
    // 0 = normal user
    // 1 = maybe a bot or anything else ?
    int         mode;
    std::string nickname;
    std::string username;
    std::string realname;
    std::string hostname;
    std::string in_buffer;
    std::queue<Message> out_messages;
;
public:
	// constructors and destructor
    Client();
    Client(int socket);
    ~Client();
	
	// setters
	void		setVerified(bool verified);
	// void		setNickname(const std::string &nickname);
	// void		setUsername(const std::string &username);
	// void		setRealname(const std::string &realname);
	// void		setHostname(const std::string &hostname);
	// // getters
	int			getSocket() const;
	bool		getVerified() const;
	bool		getRegistered() const;
	std::string	getNickname() const;
	// std::string	getUsername() const;
	// std::string	getRealname() const;
	// std::string	getHostname() const;
	std::string getUserIdent() const;
	
	// methods:
	// process the incoming string from the client
    void        processInput(const std::string &msg);
	// gets the next message to send to the client from the out_messages queue
	// throws an exception if the queue is empty
	void		addOutMessage(const Message &msg);
    Message     getOutMessage();
};

// helper functions
void printMessage(const Message msg);

#endif
