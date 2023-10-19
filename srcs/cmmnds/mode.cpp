/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 19:35:40 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/19 14:58:21 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"

// ERR_NEEDMOREPARAMS     
// ERR_CHANOPRIVSNEEDED		
// ERR_USERNOTINCHANNEL     
// ERR_UNKNOWNMODE				
// RPL_CHANNELMODEIS							             				          
// ERR_NOSUCHCHANNEL
// ERR_NOTONCHANNEL

// +i				set channel to invite-only
// -i				remove invite-only status from channel
void		modeI(Channel &chan, char sign, std::string rpls[])
{
	if (sign == '+')
	{
		chan.i = true;
		rpls[0] += 'i';
	}
	else if (sign == '-')
	{
		chan.i = false;
		rpls[1] += 'i';
	}
}

// +t				topics are settable by channel operator only
// -t				topics may be set by anyone
void		modeT(Channel &chan, char sign, std::string rpls[])
{
	if (sign == '+')
	{
		chan.t = true;
		rpls[0] += 't';
	}
	else if (sign == '-')
	{
		chan.t = false;
		rpls[1] += 't';
	}
}

// +k <key>			set channel key (password)
// -k				remove channel key (password)
void		modeK(Message &msg, User &usr, Channel &chan, char sign, std::string rpls[])
{
	if (sign == '+')
	{
		if (msg.getParams().size() < 3)
			usr.addOutMessage(Message::fromString(ERR_GENERAL_CHANNEL(usr, chan.getName(), " +k --- Missing argument")));	
		else
		{
			chan.k = true;
			chan.setKey(msg.getParams()[2]);
			rpls[0] += 'k';
			rpls[2] += " " + msg.getParams()[2];
			msg.delParam(2);
		}
	}
	else if (sign == '-')
	{
		chan.k = false;
		rpls[1] += 'k';
		chan.setKey("");
	}
}

// +o <nick>		give channel operator privileges
// -o <nick>		remove channel operator privileges
void		modeO(Message &msg, User &usr, Channel &chan, char sign, std::string rpls[])
{
	std::string nick;
	std::string	nick_upper;
	if (msg.getParams().size() < 3)
		usr.addOutMessage(Message::fromString(ERR_GENERAL_CHANNEL(usr, chan.getName(), " " + sign + "o --- Missing argument")));
	else
	{
		nick_upper = msg.getParams()[2];
		std::transform(nick_upper.begin(), nick_upper.end(), nick_upper.begin(), ::toupper);
		if (server->nick_to_sock.find(nick_upper) == server->nick_to_sock.end())
		{
			usr.addOutMessage(Message::fromString(ERR_USERNOTINCHANNEL(usr, msg.getParams()[2], chan.getName())));
			msg.delParam(2);
			return ;
		}
		nick = server->users[server->nick_to_sock[nick_upper]].getNickname();
		if (sign == '+')
		{
			if (chan.isOperator(nick))
				msg.delParam(2);
			else
			{
				chan.operators.push_back(nick);
				rpls[0] += 'o';
				rpls[2] += " " + msg.getParams()[2];
				msg.delParam(2);
			}
		}
		else if (sign == '-')
		{
			if (!chan.isOperator(nick))
				msg.delParam(2);
			else
			{
				chan.operators.erase(std::remove(chan.operators.begin(), chan.operators.end(), nick), chan.operators.end());
				rpls[1] += 'o';
				rpls[2] += " " + msg.getParams()[2];
				msg.delParam(2);
			}
		}
	}
}

// +l <limit>		set user limit to channel
// -l				remove user limit from channel
void		modeL(Message &msg, User &usr, Channel &chan, char sign, std::string rpls[])
{
	int limit;
	std::stringstream ss;
	std::string	buff;
	
	if (sign == '+')
	{
		if (msg.getParams().size() < 3)
			usr.addOutMessage(Message::fromString(ERR_GENERAL_CHANNEL(usr, chan.getName(), " +l --- Missing argument")));
		else
		{
			buff = msg.getParams()[2];
			msg.delParam(2);
			limit = std::atoi(buff.c_str());
			if (limit <= 0)
			{
				usr.addOutMessage(Message::fromString(ERR_GENERAL_CHANNEL(usr, chan.getName(), " +l --- Invalid limit")));
				return ;	
			}
			chan.setLimit(limit);
			chan.l = true;
			rpls[0] += 'l';
			ss << limit;
			rpls[2] += " " + ss.str();
		}
	}
	else if (sign == '-')
	{
		chan.l = false;
		rpls[1] += 'l';
		chan.setLimit(0);
	}
}

// function to loop through mode arguments and set/remove modes
void		execModeParams(Message &msg, User &usr, Channel &chan, std::string rpls[])
{
	char	sign = 0;
	char	mode;
	for (size_t i = 0; i < msg.getParams()[1].size(); i++)
	{
		mode = msg.getParams()[1][i];
		if (mode == '+')
			sign = '+';
		else if (mode == '-')
			sign = '-';
		// i: Set/remove Invite-only channel
		else if (mode == 'i')
			modeI(chan, sign, rpls);
		// t: Set/remove the restrictions of the TOPIC command to channel operators
		else if (mode == 't')
			modeT(chan, sign, rpls);
		// k: Set/remove the channel key (password) 
		else if (mode == 'k')
			modeK(msg, usr, chan, sign, rpls);
		// o: Give/take channel operator privilege
		else if (mode == 'o')
			modeO(msg, usr, chan, sign, rpls);
		// l: Set/remove the user limit to channel
		else if (mode == 'l')
			modeL(msg, usr, chan, sign, rpls);
		else
			usr.addOutMessage(Message::fromString(ERR_UNKNOWNMODE(usr, mode, chan.getName())));
	}
}
		   
void		mode(Message &msg, User &usr)
{
	std::string reply;
	std::string rpls[3]; // 0 = plus-reply , 1 = minus-reply, 2 = params
	if (msg.getParams().size() < 1)
		usr.addOutMessage(Message::fromString(ERR_NEEDMOREPARAMS(usr, msg.getCommand())));
	else
	{
		std::string	channel = msg.getParams()[0];
		std::string	channel_upper = channel;
		std::transform(channel_upper.begin(), channel_upper.end(), channel_upper.begin(), ::toupper);
		// check if channel exists
		if (server->channels.find(channel_upper) == server->channels.end())
		{
			usr.addOutMessage(Message::fromString(ERR_NOSUCHCHANNEL(usr, channel)));
			return ;
		}
		Channel &chan = server->channels[channel_upper];
		// check if there is no mode argument
		if (msg.getParams().size() == 1)
			usr.addOutMessage(Message::fromString(RPL_CHANNELMODEIS(usr, channel, chan.getModes(usr))));
		// check if user is in channel
		else if (find(chan.users.begin(), chan.users.end(), usr.getNickname()) == chan.users.end())
			usr.addOutMessage(Message::fromString(ERR_NOTONCHANNEL(usr, channel)));
		// check if user is channel operator
		else if (find(chan.operators.begin(), chan.operators.end(), usr.getNickname()) == chan.operators.end())
			usr.addOutMessage(Message::fromString(ERR_CHANOPRIVSNEEDED(usr, channel)));
		// iterate through mode arguments and set/remove modes
		else
		{
			execModeParams(msg, usr, chan, rpls);
			// send reply
			if (!rpls[0].empty())
				reply = "+" + rpls[0];
			if (!rpls[1].empty())
				reply = reply + "-" + rpls[1];
			if (!rpls[2].empty())
				reply = reply + " " + rpls[2];
			if (!reply.empty())
				chan.sendMessage(Message::fromString(RPL_CHANNELMODEIS(usr, chan.getName(), reply)));
		}	
	}
}
