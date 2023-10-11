/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 15:48:32 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/11 16:34:25 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"

// This is a copy of privmsg.cpp, but with the command changed to NOTICE and it doesn't reply to the sender

void	notice(Message &msg, User &usr)
{
	if (msg.getParams().size() == 0 || (msg.getTrailing().size() == 0 && msg.getParams().size() < 2))
		return ;
	else
	{
		if (msg.getTrailing().size() == 0)
			msg.setTrailing(msg.getParams()[1]);
		std::string trgt;
		std::string trgt_upper;
		std::string targets = msg.getParams()[0];
		std::istringstream targets_stream(targets);
		while(std::getline(targets_stream, trgt, ','))
		{
			trgt_upper = trgt;
			std::transform(trgt_upper.begin(), trgt_upper.end(), trgt_upper.begin(), ::toupper);
			if (trgt[0] == '#' || trgt[0] == '&')
			{
				if (server->channels.find(trgt_upper) != server->channels.end())
					server->channels[trgt_upper].sendMessage(Message::fromString(":" + usr.getUserIdent() + " PRIVMSG " + trgt + " :" + msg.getTrailing()));
			}
			else
			{
				if (server->nick_to_sock.find(trgt_upper) != server->nick_to_sock.end())
					server->users[server->nick_to_sock[trgt_upper]].addOutMessage(Message::fromString(":" + usr.getUserIdent() + " PRIVMSG " + trgt + " :" + msg.getTrailing()));
			}
		}
	}
}
