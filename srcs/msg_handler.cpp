/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 13:14:26 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/04 18:57:45 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/msg_handler.hpp"

void	handleMessage(Message &msg, User &usr)
{
	if (msg.getCommand() == "CAP") // ignore CAP messages
		return ;
	if (usr.getVerified() == false) // only allow PASS command
	{
		if (msg.getCommand() == "PASS")
			pass(msg, usr);
		else
			server->removeUser(usr.getSocket());
	}
	else if (usr.getRegistered() == false) // only allow NICK and USER
	{
		if (msg.getCommand() == "NICK")
			nick(msg, usr);
		// else if (msg.getCommand() == "USER")
		// 	user(msg, usr);
	}
	else
	{
	}
}