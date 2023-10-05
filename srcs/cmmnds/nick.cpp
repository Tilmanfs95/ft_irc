/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 15:51:18 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/05 11:01:43 by tfriedri         ###   ########.fr       */
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
        if (usr.getNickname().empty())
        {
            usr.setNickname(nick);
            if (usr.getUsername().empty() == false && usr.getRealname().empty() == false)
                server->registerUser(usr.getSocket());
        }
        else
        {
            if (usr.getRegistered() == true)
            {
                // send nick change message to all users in all channels the user is in:
                //...
                // send nick change message the user itself:
                usr.addOutMessage(Message::fromString(":" + usr.getUserIdent() + " NICK :" + nick + "\r\n"));
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
}