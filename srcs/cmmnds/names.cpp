/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tilmanfs <tilmanfs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 13:43:00 by tfriedri          #+#    #+#             */
/*   Updated: 2023/11/20 20:59:05 by tilmanfs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"

void	names(Message &msg, User &usr)
{
	Server	*server = &Server::getInstance();
	if (msg.getParams().size() == 0) // not possible in WeeChat...
	{
		// set param[0] to a string of all channels on the server (comma separated)
		std::string all = "";
		for (std::map<std::string, Channel>::iterator it = server->channels.begin(); it != server->channels.end(); it++)
		{
			all += it->first;
			if (it != server->channels.end())
				all += ",";
		}
		msg.addParam(all);
	}
	else
	{
		// iterate over given channels
		std::string	channel;
		std::string channel_upper;
		std::string channels = msg.getParams()[0];
		std::istringstream channels_stream(channels);
		while (std::getline(channels_stream, channel, ','))
		{
			channel_upper = channel;
			for (std::string::iterator it = channel_upper.begin(); it != channel_upper.end(); ++it) {
				*it = std::toupper(static_cast<unsigned char>(*it));
			}
			// check if channel exists
			if (server->channels.find(channel_upper) != server->channels.end())
			{
				// create the string of users
				std::string users = "";
				for (size_t i = 0; i < server->channels[channel_upper].users.size(); i++)
				{
					if (std::find(server->channels[channel_upper].operators.begin(), server->channels[channel_upper].operators.end(), server->channels[channel_upper].users[i]) != server->channels[channel_upper].operators.end())
						users += "@";
					users += server->channels[channel_upper].users[i];
					if (i < server->channels[channel_upper].users.size() - 1)
						users += " ";
				}
				usr.addOutMessage(Message::fromString(RPL_NAMREPLY(usr, channel, users)));
				usr.addOutMessage(Message::fromString(RPL_ENDOFNAMES(usr, channel)));
			}
		}
	}
}
