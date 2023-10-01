/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 01:42:48 by tfriedri          #+#    #+#             */
/*   Updated: 2023/09/29 19:32:01 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <iostream>
#include <vector>
#include <sstream>

class Message
{
    // example of a full message string from server to client:
    //      :<prefix><command><params>:<trailing>
    //      :Nick!user@host.com PRIVMSG #channel :Hello there!
    //      :Nick!user@host.com JOIN #Channel
    
    // example of a full message string from client to server:
    //      <command><params>
    //      PRIVMSG #channel :Hello there!
    //      PRIVMSG Nick :Hello there!
    //      PRIVMSG #channel1,#channel2,#channel3,user1,user2 :Hello everyone!
    //      JOIN #Channel1,#Channel2,#Channel3

    
private:
    // prefix (optional) (starts with ':')
    // :<Nick>!<User>@<Host> 
    // -information about the sender
    std::string prefix;
    // -command (required)
    std::string command;
    // -params (optional)
    std::vector<std::string> params;
    // -trailing (optional) (starts with ':')
    std::string trailing;

    // private methods
    // parseMessage() parses the message string and sets the attributes
    // void   parseMessage(const std::string &message);
public:
    Message(/* args */);
    ~Message();
    
    // Setters
    void    setPrefix(const std::string &prefix);
    void    setCommand(const std::string &command);
    void    setParams(const std::vector<std::string> &params);
    void    addParam(const std::string &param);
    void    setTrailing(const std::string &trailing);
    
    // Getters
    std::string     getPrefix() const;
    std::string     getCommand() const;
    std::vector<std::string>    getParams() const;
    std::string     getTrailing() const;

    // Methods
    static Message  fromString(const std::string &msg);
    std::string     toString() const;
};

#endif
