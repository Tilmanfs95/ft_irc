/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 13:18:59 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/10 17:48:36 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"

// ERR_NEEDMOREPARAMS
// ERR_BANNEDFROMCHAN -- not needed
// ERR_INVITEONLYCHAN              
// ERR_BADCHANNELKEY
// ERR_CHANNELISFULL               
// ERR_BADCHANMASK
// ERR_NOSUCHCHANNEL -- not needed              
// ERR_TOOMANYCHANNELS -- not needed ?!
// ERR_TOOMANYTARGETS -- what is this ?!             
// ERR_UNAVAILRESOURCE -- what is this ?!
// RPL_TOPIC


void	leaveAllChannels(User &usr)
{
	std::string channel;
	for (size_t i = 0; i < usr.channels.size(); i++)
	{
		channel = usr.channels[i];
		server->channels[channel].removeUser(usr, "");
		usr.addOutMessage(Message::fromString(":" + usr.getUserIdent() + " PART " + channel));
		// check if channel is empty
		if (server->channels[channel].users.size() == 0)
		{
		    server->channels.erase(channel);
		    std::cout << "Channel " << channel << " removed because it was empty" << std::endl;
			i--; // because the channel was removed, the next channel is now at the same index
		}
	}
}

void	join(Message &msg, User &usr)
{
	if (msg.getParams().size() == 0)
	{
		usr.addOutMessage(Message::fromString(ERR_NEEDMOREPARAMS(usr, msg.getCommand())));
		return ;
	}
	if (msg.getParams().size() == 1 && msg.getParams()[0] == "0")
	{
		leaveAllChannels(usr);
		return ;
	}
	std::string	channel;
	std::string	key;
	std::istringstream channels(msg.getParams()[0]);
	std::istringstream keys(msg.getParams().size() > 1 ? msg.getParams()[1] : "");
	while (std::getline(channels, channel, ','))
	{
		if (std::getline(keys, key, ',') == false)
			key = "";
		if (channel[0] != '#' && channel[0] != '&')
			usr.addOutMessage(Message::fromString(ERR_BADCHANMASK(usr, channel)));
		else if (channel.size() > 50 || channel.find_first_of("\a\b\f\n\r\t\v") != std::string::npos)
			usr.addOutMessage(Message::fromString(ERR_ERRONEUSCHANNELNAME(usr, channel)));
		else if (key.size() > 50 || key.find_first_of("\a\b\f\n\r\t\v") != std::string::npos)
			usr.addOutMessage(Message::fromString(ERR_ERRONEUSCHANNELKEY(usr, channel)));
		else if (server->channels.find(channel) != server->channels.end())
			server->channels[channel].addUser(usr, key, false);
		else
		{
			server->channels.insert(std::pair<std::string, Channel>(channel, Channel(channel, key)));
			server->channels[channel].addUser(usr, key, true);
		}
	}
}
