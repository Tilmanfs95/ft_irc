/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tilmanfs <tilmanfs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 15:51:18 by tfriedri          #+#    #+#             */
/*   Updated: 2023/11/20 23:40:23 by tilmanfs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"

bool    check_nick_chars(std::string nick)
{
    if (nick.size() > 9)
        return false;
    for (size_t i = 0; i < nick.size(); i++)
    {
		if (i == 0 && (nick[i] == '#' || nick[i] == '&'))
			return false;
        if (nick[i] == ',' || nick[i] == '*' || nick[i] == '?' || nick[i] == '!' || nick[i] == '@')
            return false;
    }
    return true;
}

void    nick(Message &msg, User &usr)
{
    Server  *server = &Server::getInstance();
    std::string nick;
    std::string nick_upper;
    if (msg.getParams().size() == 0)
    {
        usr.addOutMessage(Message::fromString(ERR_NONICKNAMEGIVEN(usr)));
        return ;
    }
    nick = msg.getParams()[0];
    nick_upper = nick;
    for (std::string::iterator it = nick_upper.begin(); it != nick_upper.end(); ++it) {
        *it = std::toupper(static_cast<unsigned char>(*it));
    }
    if (check_nick_chars(nick) == false)
        usr.addOutMessage(Message::fromString(ERR_ERRONEUSNICKNAME(usr, nick)));
    else if (server->nick_to_sock.find(nick_upper) != server->nick_to_sock.end())
        usr.addOutMessage(Message::fromString(ERR_NICKNAMEINUSE(usr, nick)));
    else
    {
        if (usr.getRegistered() == true)
        {
            std::cout << usr.getNickname() << " changed nickname to " << nick << std::endl;
            
            for (std::vector<std::string>::iterator it = usr.channels.begin(); it != usr.channels.end(); it++)
            {
                std::string channel_upper = *it;
                for (std::string::iterator it = channel_upper.begin(); it != channel_upper.end(); ++it) {
                    *it = std::toupper(static_cast<unsigned char>(*it));
                }
                if (std::find(server->channels[channel_upper].users.begin(), server->channels[channel_upper].users.end(), usr.getNickname()) != server->channels[channel_upper].users.end())
                {
                    // erase old nickname from channel
                    server->channels[channel_upper].users.erase(std::find(server->channels[channel_upper].users.begin(), server->channels[channel_upper].users.end(), usr.getNickname()));
                    // send message to channel that user changed nickname
                    server->channels[channel_upper].sendMessage(Message::fromString(":" + usr.getUserIdent() + " NICK :" + nick));
                    // add new nickname to channel
                    server->channels[channel_upper].users.push_back(nick);
                }
            }
            // send message to user that nickname was changed
            usr.addOutMessage(Message::fromString(":" + usr.getUserIdent() + " NICK :" + nick));
            // change the nickname in the server->nick_to_sock map
            server->nick_to_sock.insert(std::pair<std::string, int>(nick_upper, usr.getSocket()));
            std::string old_nick_upper = usr.getNickname();
            for (std::string::iterator it = old_nick_upper.begin(); it != old_nick_upper.end(); ++it) {
                *it = std::toupper(static_cast<unsigned char>(*it));
            }
            server->nick_to_sock.erase(old_nick_upper);
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