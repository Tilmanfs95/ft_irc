/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 01:59:10 by tfriedri          #+#    #+#             */
/*   Updated: 2023/09/30 02:16:52 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <vector>

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
private:
    int         socket;
    bool        registered;
    int         mode;
    std::string nickname;
    std::string username;
    std::string realname;
    std::string hostname;
    // mode:
    // 0 = normal user
    // 1 = maybe a bot or anything else ?
public:
    Client();
    Client(int socket);
    ~Client();
};

#endif
