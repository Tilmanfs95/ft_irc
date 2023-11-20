/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tilmanfs <tilmanfs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 16:11:10 by tfriedri          #+#    #+#             */
/*   Updated: 2023/11/20 23:42:09 by tilmanfs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"

void	topic(Message &msg, User &usr)
{
	Server	*server = &Server::getInstance();
	if (msg.getParams().size() == 0)
		usr.addOutMessage(Message::fromString(ERR_NEEDMOREPARAMS(usr, msg.getCommand())));
	else if (msg.trailingExists() == false && msg.getParams().size() == 1)
	{
		std::string channel = msg.getParams()[0];
		std::string channel_upper = channel;
		for (std::string::iterator it = channel_upper.begin(); it != channel_upper.end(); ++it) {
			*it = std::toupper(static_cast<unsigned char>(*it));
		}
		// check if user is in channel
		if (server->channels.find(channel_upper) == server->channels.end()
			|| server->channels[channel_upper].isUser(usr.getNickname()) == false)
			usr.addOutMessage(Message::fromString(ERR_NOTONCHANNEL(usr, channel)));
		// return the topic to the user
		else
		{
			std::string topic = server->channels[channel_upper].getTopic();
			if (topic.empty())
				usr.addOutMessage(Message::fromString(RPL_NOTOPIC(usr, channel)));
			else
				usr.addOutMessage(Message::fromString(RPL_TOPIC(usr, channel, topic)));
		}
	}
	else
	{
		std::string channel = msg.getParams()[0];
		std::string channel_upper = channel;
		for (std::string::iterator it = channel_upper.begin(); it != channel_upper.end(); ++it) {
			*it = std::toupper(static_cast<unsigned char>(*it));
		}
		// check if user is in channel
		if (server->channels.find(channel_upper) == server->channels.end()
			|| server->channels[channel_upper].isUser(usr.getNickname()) == false)
			usr.addOutMessage(Message::fromString(ERR_NOTONCHANNEL(usr, channel)));
		// check if user is allowed to set the topic (is operator or channels mode is not +t)
		else if (server->channels[channel_upper].t == true
			&& server->channels[channel_upper].isOperator(usr.getNickname()) == false)
			usr.addOutMessage(Message::fromString(ERR_CHANOPRIVSNEEDED(usr, channel)));
		// set the topic and send a message to all users in the channel
		else
		{
			server->channels[channel_upper].setTopic(msg.getTrailing());
			server->channels[channel_upper].sendMessage(Message::fromString(":" + usr.getUserIdent() + " TOPIC " + channel + " :" + msg.getTrailing()));
		}
	}
}
