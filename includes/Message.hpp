/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 01:42:48 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/18 22:07:50 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include "defines.hpp"

class Message
{
    // example of a full message string from server to client:
    //      :<prefix><command><params>:<trailing>
    //      :Nick!user@host.com PRIVMSG #channel :Hello there!
    //      :Nick!user@host.com JOIN #Channel
    
    // example of a full message string from client to server:
    //      <command><params>
    //      PRIVMSG #channel H:ello there!
    //      PRIVMSG Nick :Hello there!
    //      PRIVMSG #channel1,#channel2,#channel3,user1,user2 :Hello everyone!
    //      JOIN #Channel1,#Channel2,#Channel3

    
private:
    // prefix (optional) (starts with ':')
    // :<Nick>!<User>@<Host> 
    // -information about the sender
    std::string prefix;
    // the command (mandatory)
    std::string command;
    // a vector of parameters (optional)
    std::vector<std::string> params;
	// true if the message has a trailing (starts with ':')
	// important because an empty trailing is not the same as no trailing
	bool		trailing_exists;
    // the trailing (optional)
    std::string trailing;

    // private methods
    // parseMessage() parses the message string and sets the attributes
    // void   parseMessage(const std::string &message);
public:
    Message(/* args */);
    ~Message();
    
    // Setters

	// sets the prefix
    void    setPrefix(const std::string &prefix);
	// sets the command
    void    setCommand(const std::string &command);
	// sets the params vector
    void    setParams(const std::vector<std::string> &params);
	// adds a parameter at the end of the params vector
    void    addParam(const std::string &param);
	// deletes a parameter at the given index
	void	delParam(size_t index);
	// sets the trailing
    void    setTrailing(const std::string &trailing);
    
    // Getters
	
	// returns the prefix as a string
    std::string     getPrefix() const;
	// returns the command as a string
    std::string     getCommand() const;
	// returns the params as a vector of strings
    std::vector<std::string>    getParams() const;
	// returns true if the message has a trailing
	bool						hasTrailing() const;
	// returns the trailing
    std::string     getTrailing() const;

    // Methods

	// creates a Message object from a string
    static Message  fromString(const std::string &msg);
	// returns the message as a string
    std::string     toString() const;

    // DEBUG
    void            print() const;
};

#endif
