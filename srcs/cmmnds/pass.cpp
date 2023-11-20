/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tilmanfs <tilmanfs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 15:18:07 by tfriedri          #+#    #+#             */
/*   Updated: 2023/11/20 23:40:58 by tilmanfs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"

void    pass(Message &msg, User &usr)
{
    Server  *server = &Server::getInstance();
    if (usr.getVerified() == true)
        usr.addOutMessage(Message::fromString(ERR_ALREADYREGISTRED(usr)));
    else if (msg.getParams().size() == 0)
        usr.addOutMessage(Message::fromString(ERR_NEEDMOREPARAMS(usr, "PASS")));
    else if (msg.getParams().size() == 1)
    {
        if (msg.getParams()[0] == server->getPassword())
            usr.setVerified(true);
        else
            server->removeUser(usr.getSocket());
    }
    else
        server->removeUser(usr.getSocket());
}
