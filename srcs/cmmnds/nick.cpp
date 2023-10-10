/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 15:51:18 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/10 01:41:01 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"

// ERR_NONICKNAMEGIVEN
// ERR_ERRONEUSNICKNAME
// ERR_NICKNAMEINUSE
// ERR_NICKCOLLISION -- only for multiple servers (not implemented)
// ERR_UNAVAILRESOURCE -- only if nick is reserved by nick delay mechanism (not implemented)
// ERR_RESTRICTED -- only if nick is restricted (not implemented)


bool    check_nick_chars(std::string nick)
{
    if (nick.size() > 9)
        return false;
    for (size_t i = 0; i < nick.size(); i++)
    {
        if (nick[i] == ',' || nick[i] == '*' || nick[i] == '?' || nick[i] == '!' || nick[i] == '@')
            return false;
    }
    return true;
}

void    nick(Message &msg, User &usr)
{
    std::string nick;
    if (msg.getParams().size() == 0)
        usr.addOutMessage(Message::fromString(ERR_NONICKNAMEGIVEN(usr)));
    nick = msg.getParams()[0];
    if (check_nick_chars(nick) == false)
        usr.addOutMessage(Message::fromString(ERR_ERRONEUSNICKNAME(usr, nick)));
    else if (server->nickUnused(nick) == false)
        usr.addOutMessage(Message::fromString(ERR_NICKNAMEINUSE(usr, nick)));
    else
    {
        if (usr.getRegistered() == true)
        {
            std::cout << usr.getNickname() << " changed nickname to " << nick << std::endl;
            
            for (std::vector<std::string>::iterator it = usr.channels.begin(); it != usr.channels.end(); it++)
            {
                if (std::find(server->channels[*it].users.begin(), server->channels[*it].users.end(), usr.getNickname()) != server->channels[*it].users.end())
                {
                    // here we send the message too often to the user itself !! NEEDS FIXING !!
                    // maybe we should check inside the sendMessage function if the user is in the channel
                    // ..
                    // ..
                    server->channels[*it].sendMessage(Message::fromString(":" + usr.getUserIdent() + " NICK :" + nick + "\r\n"));
                    server->channels[*it].users.erase(std::find(server->channels[*it].users.begin(), server->channels[*it].users.end(), usr.getNickname()));
                    server->channels[*it].users.push_back(nick);
                    // send message to channel
                    // server->channels[*it].sendMessage(Message::fromString(":" + usr.getUserIdent() + " NICK :" + nick + "\r\n"));
                }
            }
            // change the nickname in the server->nick_to_sock map
            server->nick_to_sock.insert(std::pair<std::string, int>(nick, usr.getSocket()));
            server->nick_to_sock.erase(usr.getNickname());
            // first send the messages and then change the nickname so that the sender of the message is still the old nickname
            usr.setNickname(nick);
        }
        else
        {
            usr.setNickname(nick);
            if (usr.getUsername().empty() == false && usr.getRealname().empty() == false)
                server->registerUser(usr.getSocket());
        } 
    }
}