/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 15:51:18 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/11 15:18:22 by tfriedri         ###   ########.fr       */
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
		if (i == 0 && (nick[i] == '#' || nick[i] == '&')) // also check for other channel prefixes that we don't support ??
			return false;
        if (nick[i] == ',' || nick[i] == '*' || nick[i] == '?' || nick[i] == '!' || nick[i] == '@')
            return false;
    }
    return true;
}

void    nick(Message &msg, User &usr)
{
    std::string nick;
    std::string nick_upper;
    if (msg.getParams().size() == 0)
        usr.addOutMessage(Message::fromString(ERR_NONICKNAMEGIVEN(usr)));
    nick = msg.getParams()[0];
    nick_upper = nick;
    std::transform(nick_upper.begin(), nick_upper.end(), nick_upper.begin(), ::toupper);
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
                std::transform(channel_upper.begin(), channel_upper.end(), channel_upper.begin(), ::toupper);
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
            std::transform(old_nick_upper.begin(), old_nick_upper.end(), old_nick_upper.begin(), ::toupper);
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