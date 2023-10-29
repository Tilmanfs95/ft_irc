/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 14:10:41 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/29 15:02:23 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"

void	quit(Message &msg, User &usr)
{
	Server	*server = &Server::getInstance();
	std::string	quit_msg;
	if (msg.getParams().size() >= 1)
		quit_msg = msg.getParams()[0];
	else
		quit_msg = msg.getTrailing();
	// this message never reaches the client because the connection is closed before.... change it ??
	usr.addOutMessage(Message::fromString("ERROR :Closing Link: " + usr.getNickname() + "(" + (usr.getUsername().empty()? "unknown" : usr.getUsername()) + "@" + usr.getHostIp() + ")" + "[" + quit_msg + "]\r\n"));
	//
	// send quit_msg to other users from same channels ??
	// ...
	server->removeUser(usr.getSocket());
}
