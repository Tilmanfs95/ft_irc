/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tilmanfs <tilmanfs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 17:05:12 by tfriedri          #+#    #+#             */
/*   Updated: 2023/11/20 21:01:39 by tilmanfs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"

void 	part(Message &msg, User &usr)
{
	Server	*server = &Server::getInstance();
	if (msg.getParams().empty())
		usr.addOutMessage(Message::fromString(ERR_NEEDMOREPARAMS(usr, msg.getCommand())));
	else
	{
		if (msg.getParams().size() > 1)
			msg.setTrailing(msg.getParams()[1]);
		std::string trgt;
		std::string trgt_upper;
		std::string targets = msg.getParams()[0];
		std::istringstream targets_stream(targets);
		while(std::getline(targets_stream, trgt, ','))
		{
			trgt_upper = trgt;
			for (std::string::iterator it = trgt_upper.begin(); it != trgt_upper.end(); ++it) {
                *it = std::toupper(static_cast<unsigned char>(*it));
            }
			if (server->channels.find(trgt_upper) != server->channels.end())
			{
				// check if user is in channel
				if (std::find(server->channels[trgt_upper].users.begin(), server->channels[trgt_upper].users.end(), usr.getNickname()) != server->channels[trgt_upper].users.end())
				{
					// send PART message to the user
					usr.addOutMessage(Message::fromString(":" + usr.getUserIdent() + " PART " + trgt + " :" + msg.getTrailing()));
					// send PART message to the channel
					server->channels[trgt_upper].removeUser(usr, msg.getTrailing());
					// check if channel is empty
					if (server->channels[trgt_upper].users.size() == 0)
					{
						server->channels.erase(trgt_upper);
						std::cout << "Channel " << trgt_upper << " removed because it was empty" << std::endl;
					}
				}
				else
					usr.addOutMessage(Message::fromString(ERR_NOTONCHANNEL(usr, trgt)));
			}
			else
				usr.addOutMessage(Message::fromString(ERR_NOSUCHCHANNEL(usr, trgt)));
		}
	}
}