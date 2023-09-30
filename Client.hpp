/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 01:59:10 by tfriedri          #+#    #+#             */
/*   Updated: 2023/09/30 15:24:19 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <vector>
#include "Message.hpp"
#include "defines.hpp"

// Questions:
// -Do we need the password? I don't think so.
// -Do we need a reclaim-time for the nickname? (for network problems)

// User registration:
//      CAP LS 302                              (optional) // some clients send this or another CAP command
//      PASS <Password>                         (optional) // password for the server
//      NICK <Nickname>                         (mandatory)
//      USER <username> <mode> * <real_name>    (mandatory) (if real_name contains spaces, it must be prefixed with ':')


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
    // messagebuffer for incoming messages
    std::string messagebuffer;
public:
    Client();
    Client(int socket);
    ~Client();
    void        processInput(const std::string &msg);
};

// helper functions
void printMessage(const Message msg);

#endif
