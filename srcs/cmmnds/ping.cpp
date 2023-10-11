/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 11:45:40 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/11 13:34:28 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"


// ERR_NEEDMOREPARAMS
// ERR_NOORIGIN


void	ping(Message &msg, User &usr)
{
	if (msg.getParams().size() == 0 && msg.getTrailing().size() == 0)
		usr.addOutMessage(Message::fromString(ERR_NEEDMOREPARAMS(usr, "PING")));
	else if (msg.getParams().size() != 0)
		usr.addOutMessage(Message::fromString("PONG " + std::string(SERVER_NAME) + " " + msg.getParams()[0]));
	else
		usr.addOutMessage(Message::fromString("PONG " + std::string(SERVER_NAME) + " " + msg.getTrailing()));
}
