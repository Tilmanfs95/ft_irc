/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tilmanfs <tilmanfs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 19:03:50 by tfriedri          #+#    #+#             */
/*   Updated: 2023/11/20 23:39:01 by tilmanfs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"

void	kick_user(User &initiator, std::string channel, std::string user, std::string comment)
{
	Server	*server = &Server::getInstance();
	std::string user_upper = user;
	for (std::string::iterator it = user_upper.begin(); it != user_upper.end(); ++it) {
		*it = std::toupper(static_cast<unsigned char>(*it));
	}
	std::string channel_upper = channel;
	for (std::string::iterator it = channel_upper.begin(); it != channel_upper.end(); ++it) {
		*it = std::toupper(static_cast<unsigned char>(*it));
	}
	// check if target user exists (silently ignore if not)
	if (server->nick_to_sock.find(user_upper) == server->nick_to_sock.end())
		return ;
	User &target = server->users[server->nick_to_sock[user_upper]];
	// check if channel exists
	if (server->channels.find(channel_upper) == server->channels.end())
		initiator.addOutMessage(Message::fromString(ERR_NOSUCHCHANNEL(initiator, channel)));
	// check if initiator is member of channel
	else if (server->channels[channel_upper].isUser(initiator.getNickname()) == false)
		initiator.addOutMessage(Message::fromString(ERR_NOTONCHANNEL(initiator, channel)));
	// check if initiator is channel operator
	else if (server->channels[channel_upper].isOperator(initiator.getNickname()) == false)
		initiator.addOutMessage(Message::fromString(ERR_CHANOPRIVSNEEDED(initiator, channel)));
	// check if user is in channel
	else if (server->channels[channel_upper].isUser(target.getNickname()) == false)
		initiator.addOutMessage(Message::fromString(ERR_USERNOTINCHANNEL(initiator, user, channel)));
	else
	{
		// check if a comment was given
		if (comment.empty())
			comment = initiator.getNickname();
		// send KICK message to the channel
		server->channels[channel_upper].sendMessage(Message::fromString(":" + initiator.getUserIdent() + " KICK " + channel + " " + target.getNickname() + " :" + comment));
		// kick user from channel
		server->channels[channel_upper].removeUser(server->users[target.getSocket()], comment, false);
		// check if channel is empty
		if (server->channels[channel_upper].users.size() == 0)
		{
			server->channels.erase(channel_upper);
			std::cout << "Channel " << channel_upper << " removed because it was empty" << std::endl;
		}
	}

}

void	kick(Message &msg, User &usr)
{
	if (msg.getParams().size() < 2)
		usr.addOutMessage(Message::fromString(ERR_NEEDMOREPARAMS(usr, msg.getCommand())));
	else
	{
		int	channel_count = 0;
		std::string channel;
		std::string channels = msg.getParams()[0];
		std::istringstream channels_stream(channels);
		std::string user;
		std::string users = msg.getParams()[1];
		std::istringstream users_stream(users);
		// check if we have to handle multiple channels
		while (std::getline(channels_stream, channel, ','))
			channel_count++;
		// if only one channel is given, kick all users from the list
		if (channel_count == 1)
		{
			while (std::getline(users_stream, user, ','))
				kick_user(usr, channel, user, msg.getTrailing());
		}
		// if multiple channels are given, kick all users from each given channel
		else
		{
			channels_stream.clear();
			channels_stream.str(channels);
			while (std::getline(channels_stream, channel, ','))
			{
				if (std::getline(users_stream, user, ','))
					kick_user(usr, channel, user, msg.getTrailing());
				else
					usr.addOutMessage(Message::fromString(ERR_NEEDMOREPARAMS(usr, msg.getCommand())));
			}
		}
	}
}
