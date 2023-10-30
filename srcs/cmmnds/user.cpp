/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 23:21:40 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/29 15:02:39 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"
#include <string>

// ERR_NEEDMOREPARAMS
// ERR_ALREADYREGISTRED

void    user(Message &msg, User &usr)
{
    Server  *server = &Server::getInstance();
    if (msg.getParams().size() < 3)
        usr.addOutMessage(Message::fromString(ERR_NEEDMOREPARAMS(usr, "USER")));
    else if (usr.getRegistered() == true)
        usr.addOutMessage(Message::fromString(ERR_ALREADYREGISTRED(usr)));
    else
    {
        usr.setUsername(msg.getParams()[0]);
        usr.setMode(std::atoi(msg.getParams()[1].c_str()));
        if (msg.getParams().size() > 3)
            usr.setRealname(msg.getParams()[3]);
        else
            usr.setRealname(msg.getTrailing());
        if (usr.getNickname().empty() == false)
            server->registerUser(usr.getSocket());
    }
}
