/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 19:35:40 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/18 23:35:32 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"

// ERR_NEEDMOREPARAMS              	-done
// ERR_CHANOPRIVSNEEDED				-done
// ERR_USERNOTINCHANNEL     		-done    
// ERR_UNKNOWNMODE								"<char> :is unknown mode char to me for <channel>" ???

// RPL_CHANNELMODEIS							             				          
// RPL_INVITELIST                  
// RPL_ENDOFINVITELIST


// ERR_NOSUCHCHANNEL
// ERR_NOTONCHANNEL

		   
void		mode(Message &msg, User &usr)
{
	if (msg.getParams().size() < 1)
		usr.addOutMessage(Message::fromString(ERR_NEEDMOREPARAMS(usr, msg.getCommand())));
	else if (msg.getParams().size() == 1)
	{
		// RPL_CHANNELMODEIS (if channel exists)
	}
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
		// check if user is in channel
		if (find(chan.users.begin(), chan.users.end(), usr.getNickname()) == chan.users.end())
			usr.addOutMessage(Message::fromString(ERR_NOTONCHANNEL(usr, channel)));
		// check if user is channel operator
		else if (find(chan.operators.begin(), chan.operators.end(), usr.getNickname()) == chan.operators.end())
			usr.addOutMessage(Message::fromString(ERR_CHANOPRIVSNEEDED(usr, channel)));
		else
		{
			std::string reply;
			std::string	reply_plus = "";
			std::string	reply_minus = "";
			// std::vector<std::string> reply_params;
			std::string reply_params = "";
			// iterate over modes
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
				else if (mode == 'i' && sign == '+')
				{
					chan.i = true;
					reply_plus += mode;
				}
				else if (mode == 'i' && sign == '-')
				{
					chan.i = false;
					reply_minus += mode;
				}
				
				// t: Set/remove the restrictions of the TOPIC command to channel operators
				else if (mode == 't' && sign == '+')
				{
					chan.t = true;
					reply_plus += mode;
				}
				else if (mode == 't' && sign == '-')
				{
					chan.t = false;
					reply_minus += mode;
				}
				
				// k: Set/remove the channel key (password) 
				else if (mode == 'k' && sign == '+') // NEEDS AN ARGUMENT
				{
					if (msg.getParams().size() < 3)
						usr.addOutMessage(Message::fromString(ERR_NEEDMOREPARAMS(usr, msg.getCommand() + " +k")));
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
						usr.addOutMessage(Message::fromString(ERR_NEEDMOREPARAMS(usr, msg.getCommand() + " +o")));
					else
					{
						std::string nick;
						std::string	nick_upper = msg.getParams()[2];
						std::transform(nick_upper.begin(), nick_upper.end(), nick_upper.begin(), ::toupper);
						// if (find(server->nick_to_sock.begin(), server->nick_to_sock.end(), nick_upper) == server->nick_to_sock.end())
						if (server->nick_to_sock.find(nick_upper) == server->nick_to_sock.end())
						{
							usr.addOutMessage(Message::fromString(ERR_USERNOTINCHANNEL(usr, msg.getParams()[2], channel)));
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
				// else if (mode == 'o' && sign == '-')
				// {
				// 	//
				// }
				
				// l: Set/remove the user limit to channel
				// else if (mode == 'l' && sign == '+') // NEEDS AN ARGUMENT
				// {
				// }
				// else if (mode == 'l' && sign == '-')
				// {
				// }
				
				
				else
					usr.addOutMessage(Message::fromString(ERR_UNKNOWNMODE(usr, mode, channel)));
			}
			// send RPL_CHANNELMODEIS to all users in channel
			if (!reply_plus.empty())
				reply = "+" + reply_plus;
			if (!reply_minus.empty())
				reply = reply + "-" + reply_minus;
			if (!reply_params.empty())
				reply = reply + " " +reply_params;
			chan.sendMessage(Message::fromString(RPL_CHANNELMODEIS(usr, channel, reply)));
			//
			// This message is working for all clients (processing its content) in the channel
			// but WeeChat is not showing it... why ? in ircnet it shows the message...
			//
		}
	}
}
