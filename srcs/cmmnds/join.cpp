/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 13:18:59 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/06 23:54:48 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"

// ERR_NEEDMOREPARAMS
// ERR_BANNEDFROMCHAN -- not needed
// ERR_INVITEONLYCHAN              
// ERR_BADCHANNELKEY
// ERR_CHANNELISFULL               
// ERR_BADCHANMASK
// ERR_NOSUCHCHANNEL -- not needed              
// ERR_TOOMANYCHANNELS -- not needed ?!
// ERR_TOOMANYTARGETS -- what is this ?!             
// ERR_UNAVAILRESOURCE -- what is this ?!
// RPL_TOPIC

void	join(Message &msg, User &usr)
{
	if (msg.getParams().size() == 0)
	{
		//send ERR_NEEDMOREPARAMS
		//...
		return ;
	}
	if (msg.getParams().size() == 1 && msg.getCommand()[0] == '0')
	{
		//leave all channels !
		//handle like part command for each channel
		//...
		return ;
	}
	std::istringstream iss(msg.getTrailing());
	std::string	channel;
	std::string	key;
	for (size_t i = 0; i < msg.getParams().size(); i++)
	{
		channel = msg.getParams()[i];
		std::getline(iss, key, ',');
		if (channel[0] != '#' && channel[0] != '&')
		{
			//send ERR_BADCHANMASK
			//...
			continue ;
		}
		if (server->channels.find(channel) != server->channels.end())
		{
			try
			{
				server->channels[channel].addUser(usr, key);
				// send RPL_TOPIC
				//...
			}
			catch(const std::exception& e)
			{
				// std::cerr << e.what() << '\n'; //
				if (std::string(e.what()).compare("Channel key is incorrect") == 0)
				{
					//send ERR_BADCHANNELKEY
				}
				// else if (e.what().compare("Channel is full") == 0)
				else if (std::string(e.what()).compare("Channel is full") == 0)
				{
					//send ERR_CHANNELISFULL
				}
				// else if (e.what().compare("Channel is invite only") == 0)
				else if (std::string(e.what()).compare("Channel is invite only") == 0)
				{
					//send ERR_INVITEONLYCHAN
				}
			}
			continue ;
		}
		else
		{
			try
			{
				server->addChannel(channel, key);
				server->channels[channel].addUser(usr, key);
				server->channels[channel].setAsOperator(usr.getNickname());
				//
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n'; //
				// Do we need to send an error message here?
				continue ;
			}
		}
		
	}
}