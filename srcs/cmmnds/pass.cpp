/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 15:18:07 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/04 20:42:19 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/msg_handler.hpp"

// ERR_ALREADYREGISTRED
// ERR_NEEDMOREPARAMS

void    pass(Message &msg, User &usr)
{
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
