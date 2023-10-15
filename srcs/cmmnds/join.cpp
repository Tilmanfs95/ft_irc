/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 13:18:59 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/15 20:04:43 by tfriedri         ###   ########.fr       */
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


void	leaveAllChannels(User &usr)
{
	std::string channel;
	while (usr.channels.size() > 0)
	{
		channel = usr.channels[0];
		std::transform(channel.begin(), channel.end(), channel.begin(), ::toupper);
		server->channels[channel].removeUser(usr, "");
		if (server->channels[channel].users.size() == 0)
		    server->channels.erase(channel);
	}
}

void	join(Message &msg, User &usr)
{
	if (msg.getParams().size() == 0)
	{
		usr.addOutMessage(Message::fromString(ERR_NEEDMOREPARAMS(usr, msg.getCommand())));
		return ;
	}
	if (msg.getParams().size() == 1 && msg.getParams()[0] == "0")
	{
		leaveAllChannels(usr);
		return ;
	}
	std::string	channel;
	std::string	channel_upper; // channelname in uppercase for case insensitive comparison
	std::string	key;
	std::istringstream channels(msg.getParams()[0]);
	std::istringstream keys(msg.getParams().size() > 1 ? msg.getParams()[1] : "");
	while (std::getline(channels, channel, ','))
	{
		channel_upper = channel;
		std::transform(channel_upper.begin(), channel_upper.end(), channel_upper.begin(), ::toupper);
		if (std::getline(keys, key, ',') == false)
			key = "";
		if (channel[0] != '#' && channel[0] != '&')
			usr.addOutMessage(Message::fromString(ERR_BADCHANMASK(usr, channel)));
		else if (channel.size() > 50 || channel.find_first_of(" :\a\b\f\n\r\t\v") != std::string::npos)
			usr.addOutMessage(Message::fromString(ERR_ERRONEUSCHANNELNAME(usr, channel)));
		else if (key.size() > 50 || key.find_first_of(" :\a\b\f\n\r\t\v") != std::string::npos) 
			usr.addOutMessage(Message::fromString(ERR_ERRONEUSCHANNELKEY(usr, channel)));
		else if (server->channels.find(channel_upper) != server->channels.end())
			server->channels[channel_upper].addUser(usr, key, false);
		else 
		{
			server->channels.insert(std::pair<std::string, Channel>(channel_upper, Channel(channel, key)));
			server->channels[channel_upper].addUser(usr, key, true);
		}
	}
}
