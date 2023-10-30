/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 01:59:10 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/30 13:02:01 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include "Message.hpp"
#include "Server.hpp"
#include "commands.hpp"
#include "defines.hpp"

#include <iostream>
#include <vector>
#include <queue>

// forward declaration
class Server;


class User
{
private:
    // socket fd
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
    std::string host_ip;
    // Messages that need to be sent to the client
    std::queue<Message> out_messages;
public:
    // channelnames of the channels the user is in
    // Important:
    // - case sensitive !
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
    
	// Adds a channel to the channels vector
	void        addChannel(const std::string &channel);
    // Adds a message to the out_messages queue
    void		addOutMessage(const Message &msg);
    // Returns the first message from the out_messages queue
    // and removes it
    Message     getOutMessage();
};

#endif
