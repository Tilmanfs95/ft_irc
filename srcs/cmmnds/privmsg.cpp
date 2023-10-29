/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 15:00:49 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/29 15:02:19 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/commands.hpp"



// ERR_NOSUCHNICK (401) - done
// ERR_NOSUCHSERVER (402) - not needed because we don't support multiple servers
// ERR_CANNOTSENDTOCHAN (404) - not needed because users can't be banned from channels
// ERR_TOOMANYTARGETS (407) - not needed because we don't limit the number of targets
// ERR_NORECIPIENT (411) - done
// ERR_NOTEXTTOSEND (412) - done
// ERR_NOTOPLEVEL (413) - not needed ?
// ERR_WILDTOPLEVEL (414) - not needed ?
// RPL_AWAY (301) - do we support away messages ? if not, not needed

void	privmsg(Message &msg, User &usr)
{
	Server	*server = &Server::getInstance();
	if (msg.getParams().size() == 0)
		usr.addOutMessage(Message::fromString(ERR_NORECIPIENT(usr, msg.getCommand())));
	else if (msg.getTrailing().empty() && msg.getParams().size() < 2)
		usr.addOutMessage(Message::fromString(ERR_NOTEXTTOSEND(usr)));
	else
	{
		// if (msg.getTrailing().size() == 0)
		if (msg.getParams().size() > 1)
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
					server->channels[trgt_upper].sendMessage(Message::fromString(":" + usr.getUserIdent() + " PRIVMSG " + trgt + " :" + msg.getTrailing()), usr.getNickname());
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
