/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tilmanfs <tilmanfs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 15:00:49 by tfriedri          #+#    #+#             */
/*   Updated: 2023/11/22 16:13:55 by tilmanfs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/commands.hpp"

void	privmsg(Message &msg, User &usr)
{
	Server	*server = &Server::getInstance();
	if (msg.getParams().size() == 0)
		usr.addOutMessage(Message::fromString(ERR_NORECIPIENT(usr, msg.getCommand())));
	else if (msg.getTrailing().empty() && msg.getParams().size() < 2)
		usr.addOutMessage(Message::fromString(ERR_NOTEXTTOSEND(usr)));
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
				{
					// check if user is in channel
					if (std::find(server->channels[trgt_upper].users.begin(), server->channels[trgt_upper].users.end(), usr.getNickname()) == server->channels[trgt_upper].users.end())
						usr.addOutMessage(Message::fromString(ERR_NOTONCHANNEL(usr, trgt)));
					else
					server->channels[trgt_upper].sendMessage(Message::fromString(":" + usr.getUserIdent() + " PRIVMSG " + trgt + " :" + msg.getTrailing()), usr.getNickname());
				}
				else
					usr.addOutMessage(Message::fromString(ERR_NOSUCHNICK(usr, trgt)));
			}
			else
			{
				if (server->nick_to_sock.find(trgt_upper) != server->nick_to_sock.end())
					server->users[server->nick_to_sock[trgt_upper]].addOutMessage(Message::fromString(":" + usr.getUserIdent() + " PRIVMSG " + trgt + " :" + msg.getTrailing()));
				else
					usr.addOutMessage(Message::fromString(ERR_NOSUCHNICK(usr, trgt)));
			}
		}
	}
}
