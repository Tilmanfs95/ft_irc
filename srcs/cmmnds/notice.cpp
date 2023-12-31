/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tilmanfs <tilmanfs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 15:48:32 by tfriedri          #+#    #+#             */
/*   Updated: 2023/11/20 23:40:47 by tilmanfs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"

// This is a copy of privmsg.cpp, but with the command changed to NOTICE and it doesn't reply to the sender

void	notice(Message &msg, User &usr)
{
	Server	*server = &Server::getInstance();
	if (msg.getParams().size() == 0 || (msg.getTrailing().size() == 0 && msg.getParams().size() < 2))
		return ;
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
			if (trgt[0] == '#' || trgt[0] == '&')
			{
				if (server->channels.find(trgt_upper) != server->channels.end())
					server->channels[trgt_upper].sendMessage(Message::fromString(":" + usr.getUserIdent() + " PRIVMSG " + trgt + " :" + msg.getTrailing()), usr.getNickname());
			}
			else
			{
				if (server->nick_to_sock.find(trgt_upper) != server->nick_to_sock.end())
					server->users[server->nick_to_sock[trgt_upper]].addOutMessage(Message::fromString(":" + usr.getUserIdent() + " PRIVMSG " + trgt + " :" + msg.getTrailing()));
			}
		}
	}
}
