/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 16:11:10 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/17 17:43:20 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"

// ERR_NEEDMOREPARAMS 
// ERR_NOTONCHANNEL		
// RPL_NOTOPIC    		           
// RPL_TOPIC			
// ERR_CHANOPRIVSNEEDED 

void	topic(Message &msg, User &usr)
{
	if (msg.getParams().size() == 0)
		usr.addOutMessage(Message::fromString(ERR_NEEDMOREPARAMS(usr, msg.getCommand())));
	else if (msg.hasTrailing() == false && msg.getParams().size() == 1)
	{
		std::string channel = msg.getParams()[0];
		std::string channel_upper = channel;
		std::transform(channel_upper.begin(), channel_upper.end(), channel_upper.begin(), ::toupper);
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
		std::transform(channel_upper.begin(), channel_upper.end(), channel_upper.begin(), ::toupper);
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
