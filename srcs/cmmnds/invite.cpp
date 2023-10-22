/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 16:03:48 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/22 16:32:00 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"

// RPL_INVITING (341)
// ERR_NEEDMOREPARAMS (461)   
// ERR_NOSUCHCHANNEL (403)     
// ERR_NOTONCHANNEL (442)       
// ERR_CHANOPRIVSNEEDED (482)  
// ERR_USERONCHANNEL (443)     
// ERR_NOSUCHNICK (401)         

void    invite(Message &msg, User &usr)
{
    if (msg.getParams().size() < 2)
        usr.addOutMessage(Message::fromString(ERR_NEEDMOREPARAMS(usr, msg.getCommand())));
    else
    {
        std::string target_nick = msg.getParams()[0];
        std::string channel = msg.getParams()[1];
        std::string target_user_upper = target_nick;
        std::transform(target_user_upper.begin(), target_user_upper.end(), target_user_upper.begin(), ::toupper);
        std::string channel_upper = channel;
        std::transform(channel_upper.begin(), channel_upper.end(), channel_upper.begin(), ::toupper);
        // check if target user exists
        if (server->nick_to_sock.find(target_user_upper) == server->nick_to_sock.end())
        {
            usr.addOutMessage(Message::fromString(ERR_NOSUCHNICK(usr, target_nick)));
            return ;
        }
        User &target = server->users[server->nick_to_sock[target_user_upper]];
        target_nick = target.getNickname();
        // check if channel exists
        if (server->channels.find(channel_upper) == server->channels.end())
            usr.addOutMessage(Message::fromString(ERR_NOSUCHCHANNEL(usr, channel)));
        // check if initiator is member of channel
        else if (server->channels[channel_upper].isUser(usr.getNickname()) == false)
            usr.addOutMessage(Message::fromString(ERR_NOTONCHANNEL(usr, channel)));
        // check if channel is invite only and initiator is not channel operator
        else if (server->channels[channel_upper].i == true && server->channels[channel_upper].isOperator(usr.getNickname()) == false)
            usr.addOutMessage(Message::fromString(ERR_CHANOPRIVSNEEDED(usr, channel)));
        // check if target user is already on channel
        else if (server->channels[channel_upper].isUser(target_nick) == true)
            usr.addOutMessage(Message::fromString(ERR_USERONCHANNEL(usr, target_nick, channel)));
        else
        {
            // add target user to channels invite list
            server->channels[channel_upper].invites.push_back(target_nick);
            // send reply and invitation
            target.addOutMessage(Message::fromString(":" + usr.getUserIdent() + " INVITE " + target_nick + " " + channel));
            usr.addOutMessage(Message::fromString(":" + usr.getNickname() + " 341 " + usr.getNickname() + " " + target_nick + " " + channel));
        }
    }
}
