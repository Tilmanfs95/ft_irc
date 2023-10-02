/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 13:14:26 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/02 15:13:28 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/msg_handler.hpp"

void	handleMessage(Message &msg, Client &usr)
{
	if (msg.getCommand() == "CAP") // ignore CAP messages
		return ;
	if (usr.getVerified() == false)
	{
		// only allow PASS command
		if (msg.getCommand() == "PASS" && msg.getParams().size() == 1)
		{
			if (msg.getParams()[0] == server->getPassword())
			{
				std::cout << "Password correct" << std::endl;
				usr.setVerified(true);
				// FOLLOWING ONLY FOR TESTING PURPOSES
				usr.addOutMessage(Message::fromString(RPL_WELCOME(usr))); //DO NOT REPLY TO CORRECT PASSWORD
			}
			else
			{
				// No reply but close connection
				std::cout << "Password incorrect" << std::endl;
				server->removeClient(usr.getSocket());
			}
		}
	}
	// else if (this->registered == false)
	else if (usr.getRegistered() == false)
	{
		// only allow NICK and USER commands
	}
	else
	{
		// process message
	}
}