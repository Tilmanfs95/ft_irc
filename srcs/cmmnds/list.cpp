/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 10:02:16 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/26 10:58:46 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"

// RPL_LISTSTART (321)
// RPL_LIST (322)
// RPL_LISTEND (323)

void    list(Message &msg, User &usr)
{
    // send RPL_LISTSTART
    usr.addOutMessage(Message::fromString(RPL_LISTSTART(usr)));
    if (msg.getParams().size() == 0)
    {
        // list all channels
        for (std::map<std::string, Channel>::iterator it = server->channels.begin(); it != server->channels.end(); it++)
        {
            std::stringstream chansize;
            chansize << it->second.users.size();
            usr.addOutMessage(Message::fromString(RPL_LIST(usr, it->second.getName(), chansize.str(), it->second.getTopic())));
        }
    }
    else
    {
        std::string channel;
        std::string chnnllst = msg.getParams()[0];
        std::string chnnllst_upper = chnnllst;
        std::transform(chnnllst_upper.begin(), chnnllst_upper.end(), chnnllst_upper.begin(), ::toupper);
        std::istringstream iss(chnnllst_upper);

        // send RPL_LIST for each channel
        while (std::getline(iss, channel, ','))
        {
            // check if channel exists
            if (server->channels.find(channel) != server->channels.end())
            {
                std::stringstream chansize;
                chansize << server->channels[channel].users.size();
                usr.addOutMessage(Message::fromString(RPL_LIST(usr, channel, chansize.str(), server->channels[channel].getTopic())));
            }
        }
    }
    // send RPL_LISTEND
    usr.addOutMessage(Message::fromString(RPL_LISTEND(usr)));
}