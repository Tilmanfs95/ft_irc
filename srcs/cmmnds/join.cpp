/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tilmanfs <tilmanfs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 13:18:59 by tfriedri          #+#    #+#             */
/*   Updated: 2023/11/20 23:38:45 by tilmanfs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"

void	leaveAllChannels(User &usr)
{
	Server	*server = &Server::getInstance();
	std::string channel;
	while (usr.channels.size() > 0)
	{
		channel = usr.channels[0];
		for (std::string::iterator it = channel.begin(); it != channel.end(); ++it) {
			*it = std::toupper(static_cast<unsigned char>(*it));
		}
		server->channels[channel].removeUser(usr, "");
		if (server->channels[channel].users.size() == 0)
		    server->channels.erase(channel);
	}
}

void	join(Message &msg, User &usr)
{
	Server	*server = &Server::getInstance();
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
	std::string	channel_upper; // channelname in uppercase for case insensitive comparison
	std::string	key;
	std::istringstream channels(msg.getParams()[0]);
	std::istringstream keys(msg.getParams().size() > 1 ? msg.getParams()[1] : "");
	while (std::getline(channels, channel, ','))
	{
		channel_upper = channel;
		for (std::string::iterator it = channel_upper.begin(); it != channel_upper.end(); ++it) {
			*it = std::toupper(static_cast<unsigned char>(*it));
		}
		if (std::getline(keys, key, ',') == false)
			key = "";
		if (channel[0] != '#' && channel[0] != '&')
			usr.addOutMessage(Message::fromString(ERR_BADCHANMASK(usr, channel)));
		else if (channel.size() > 50 || channel.find_first_of(" :\a\b\f\n\r\t\v") != std::string::npos)
			usr.addOutMessage(Message::fromString(ERR_ERRONEUSCHANNELNAME(usr, channel)));
		else if (key.size() > 50 || key.find_first_of(" :\a\b\f\n\r\t\v") != std::string::npos) 
			usr.addOutMessage(Message::fromString(ERR_ERRONEUSCHANNELKEY(usr, channel)));
		else if (server->channels.find(channel_upper) != server->channels.end())
			server->channels[channel_upper].addUser(usr, key, false);
		else 
		{
			server->channels.insert(std::pair<std::string, Channel>(channel_upper, Channel(channel, key)));
			server->channels[channel_upper].addUser(usr, key, true);
		}
	}
}
