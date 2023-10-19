/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 19:35:40 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/19 14:54:35 by tfriedri         ###   ########.fr       */
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
// +t				topics are settable by channel operator only
// -t				topics may be set by anyone
// +k <key>			set channel key (password)
// -k				remove channel key (password)
// +o <nick>		give channel operator privileges
// -o <nick>		remove channel operator privileges
// +l <limit>		set user limit to channel
// -l				remove user limit from channel

void		modeI(Channel &chan, char sign, std::string *reply_plus, std::string *reply_minus)
{
	if (sign == '+')
	{
		chan.i = true;
		*reply_plus += 'i';
	}
	else if (sign == '-')
	{
		chan.i = false;
		*reply_minus += 'i';
	}
}

void		modeT(Channel &chan, char sign, std::string *reply_plus, std::string *reply_minus)
{
	if (sign == '+')
	{
		chan.t = true;
		*reply_plus += 't';
	}
	else if (sign == '-')
	{
		chan.t = false;
		*reply_minus += 't';
	}
}

void		iterModeParams(Message &msg, User &usr, Channel &chan)
{
	std::string reply;
	std::string	reply_plus = "";
	std::string	reply_minus = "";
	std::string reply_params = "";
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
			modeI(chan, sign, &reply_plus, &reply_minus);
		// t: Set/remove the restrictions of the TOPIC command to channel operators
		else if (mode == 't')
			modeT(chan, sign, &reply_plus, &reply_minus);

		// k: Set/remove the channel key (password) 
		else if (mode == 'k' && sign == '+') // NEEDS AN ARGUMENT
		{
			if (msg.getParams().size() < 3)
				// usr.addOutMessage(Message::fromString(ERR_NEEDMOREPARAMS(usr, msg.getCommand() + " +k")));
				usr.addOutMessage(Message::fromString(ERR_GENERAL_CHANNEL(usr, chan.getName(), " +k --- Missing argument")));
			else
			{
				chan.k = true;
				chan.setKey(msg.getParams()[2]);
				reply_plus += mode;
				reply_params = reply_params + " " + msg.getParams()[2];
				msg.delParam(2);
			}
		}
		else if (mode == 'k' && sign == '-')
		{
			chan.k = false;
			reply_minus += mode;
			chan.setKey("");
		}
		
		// o: Give/take channel operator privilege
		else if (mode == 'o' && sign == '+') // NEEDS AN ARGUMENT
		{
			if (msg.getParams().size() < 3)
				// usr.addOutMessage(Message::fromString(ERR_NEEDMOREPARAMS(usr, msg.getCommand() + " +o")));
				usr.addOutMessage(Message::fromString(ERR_GENERAL_CHANNEL(usr, chan.getName(), " +o --- Missing argument")));
			else
			{
				std::string nick;
				std::string	nick_upper = msg.getParams()[2];
				std::transform(nick_upper.begin(), nick_upper.end(), nick_upper.begin(), ::toupper);
				if (server->nick_to_sock.find(nick_upper) == server->nick_to_sock.end())
				{
					usr.addOutMessage(Message::fromString(ERR_USERNOTINCHANNEL(usr, msg.getParams()[2], chan.getName())));
					msg.delParam(2);
					continue ;
				}
				nick = server->users[server->nick_to_sock[nick_upper]].getNickname();
				if (chan.isOperator(nick))
				{
					msg.delParam(2);
					continue ;	
				}
				else
				{
					chan.operators.push_back(nick);
					reply_plus += mode;
					reply_params = reply_params + " " + msg.getParams()[2];
					msg.delParam(2);
				}
			}
		}
		else if (mode == 'o' && sign == '-') // NEEDS AN ARGUMENT
		{
			if (msg.getParams().size() < 3)
				// usr.addOutMessage(Message::fromString(ERR_NEEDMOREPARAMS(usr, msg.getCommand() + " +o")));
				usr.addOutMessage(Message::fromString(ERR_GENERAL_CHANNEL(usr, chan.getName(), " +o --- Missing argument")));
			else
			{
										std::string nick;
				std::string	nick_upper = msg.getParams()[2];
				std::transform(nick_upper.begin(), nick_upper.end(), nick_upper.begin(), ::toupper);
				if (server->nick_to_sock.find(nick_upper) == server->nick_to_sock.end())
				{
					usr.addOutMessage(Message::fromString(ERR_USERNOTINCHANNEL(usr, msg.getParams()[2], chan.getName())));
					msg.delParam(2);
					continue ;
				}
				nick = server->users[server->nick_to_sock[nick_upper]].getNickname();
				if (!chan.isOperator(nick))
				{
					msg.delParam(2);
					continue ;	
				}
				else
				{
					chan.operators.erase(std::remove(chan.operators.begin(), chan.operators.end(), nick), chan.operators.end());
					reply_minus += mode;
					reply_params = reply_params + " " + msg.getParams()[2];
					msg.delParam(2);
				}
			}
		}
		
		// l: Set/remove the user limit to channel
		else if (mode == 'l' && sign == '+') // NEEDS AN ARGUMENT
		{
			if (msg.getParams().size() < 3)
				// usr.addOutMessage(Message::fromString(ERR_NEEDMOREPARAMS(usr, msg.getCommand() + " +l")));
				usr.addOutMessage(Message::fromString(ERR_GENERAL_CHANNEL(usr, chan.getName(), " +l --- Missing argument")));
			else
			{
				try
				{
					std::string	buff = msg.getParams()[2];
					msg.delParam(2);
					int limit = std::stoi(buff);
					if (limit < 0)
						throw std::exception();
					chan.setLimit(std::stoi(buff));
					chan.l = true;
					reply_plus += mode;
					reply_params = reply_params + " " + std::to_string(limit);
					
				}
				catch(const std::exception& e)
				{
					usr.addOutMessage(Message::fromString(ERR_GENERAL_CHANNEL(usr, chan.getName(), " +l --- Invalid limit")));
				}
			}
		}
		else if (mode == 'l' && sign == '-')
		{
			chan.l = false;
			reply_minus += mode;
			chan.setLimit(0);
		}
		else
			usr.addOutMessage(Message::fromString(ERR_UNKNOWNMODE(usr, mode, chan.getName())));
	}
	// send changes as RPL_CHANNELMODEIS to all users in channel
	if (!reply_plus.empty())
		reply = "+" + reply_plus;
	if (!reply_minus.empty())
		reply = reply + "-" + reply_minus;
	if (!reply_params.empty())
		reply = reply + " " +reply_params;
	if (!reply.empty())
		chan.sendMessage(Message::fromString(RPL_CHANNELMODEIS(usr, chan.getName(), reply)));
}
		   
void		mode(Message &msg, User &usr)
{
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
			iterModeParams(msg, usr, chan);
	}
}
