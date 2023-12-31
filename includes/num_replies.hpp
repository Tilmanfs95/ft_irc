/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   num_replies.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tilmanfs <tilmanfs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 13:59:06 by tfriedri          #+#    #+#             */
/*   Updated: 2023/11/22 15:25:23 by tilmanfs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUM_REPLIES_HPP
# define NUM_REPLIES_HPP
# include "User.hpp"


# define RPL_WELCOME(user) (":" SERVER_NAME " 001 " + user.getNickname() + " :Welcome to the Internet Relay Network " + user.getUserIdent())
# define RPL_YOURHOST(user) (":" SERVER_NAME " 002 " + user.getNickname() + " :Your host is " + SERVER_NAME + ", running version 1.0")
# define RPL_ISSUPPORT(user, params) (":" SERVER_NAME " 005 " + user.getNickname() + " " + params)
# define RPL_LISTSTART(user) (":" SERVER_NAME " 321 " + user.getNickname() + " Channel :Users  Name")
# define RPL_LIST(user, channel, users, topic) (":" SERVER_NAME " 322 " + user.getNickname() + " " + channel + " " + users + " :" + topic)
# define RPL_LISTEND(user) (":" SERVER_NAME " 323 " + user.getNickname() + " :End of LIST")
# define RPL_CHANNELMODEIS(user, channel, modes) (":" SERVER_NAME " 324 " + user.getNickname() + " " + channel + " " + modes)
# define RPL_NOTOPIC(user, channel) (":" SERVER_NAME " 331 " + user.getNickname() + " " + channel + " :No topic is set")
# define RPL_TOPIC(user, channel, topic) (":" SERVER_NAME " 332 " + user.getNickname() + " " + channel + " :" + topic)
# define RPL_INVITELIST(user, channel, mask) (":" SERVER_NAME " 346 " + user.getNickname() + " " + channel + " " + mask)
# define RPL_NAMREPLY(user, channel, nicknames) (":" SERVER_NAME " 353 " + user.getNickname() + " = " + channel + " :" + nicknames)
# define RPL_ENDOFNAMES(user, channel) (":" SERVER_NAME " 366 " + user.getNickname() + " " + channel + " :End of NAMES list")
# define RPL_MOTD(user, line) (":" SERVER_NAME " 372 " + user.getNickname() + " :- " + line)
# define RPL_MOTDSTART(user) (":" SERVER_NAME " 375 " + user.getNickname() + " :- " + SERVER_NAME + " Message of the day - ")
# define RPL_ENDOFMOTD(user) (":" SERVER_NAME " 376 " + user.getNickname() + " :End of MOTD command")
# define ERR_NOSUCHNICK(user, target) (":" SERVER_NAME " 401 " + user.getNickname() + " " + target + " :No such nick/channel")
# define ERR_NOSUCHCHANNEL(user, channel) (":" SERVER_NAME " 403 " + user.getNickname() + " " + channel + " :No such channel")
# define ERR_NORECIPIENT(user, command) (":" SERVER_NAME " 411 " + user.getNickname() + " :No recipient given (" + command + ")")
# define ERR_NOTEXTTOSEND(user) (":" SERVER_NAME " 412 " + user.getNickname() + " :No text to send")
# define ERR_UNKNOWNCOMMAND(user, command) (":" SERVER_NAME " 421 " + (user.getNickname().empty() ? "*" : user.getNickname()) + " " + command + " :Unknown command")
# define ERR_NONICKNAMEGIVEN(user) (":" SERVER_NAME " 431 " + (user.getNickname().empty() ? "*" : user.getNickname()) + " :No nickname given")
# define ERR_ERRONEUSNICKNAME(user, nick) (":" SERVER_NAME " 432 " + (user.getNickname().empty() ? "*" : user.getNickname()) + " " + nick + " :Erroneus nickname")
# define ERR_NICKNAMEINUSE(user, nick) (":" SERVER_NAME " 433 " + (user.getNickname().empty() ? "*" : user.getNickname()) + " " + nick + " :Nickname is already in use")
# define ERR_USERNOTINCHANNEL(user, target, channel) (":" SERVER_NAME " 441 " + user.getNickname() + " " + target + " " + channel + " :They aren't on that channel")
# define ERR_NOTONCHANNEL(user, channel) (":" SERVER_NAME " 442 " + user.getNickname() + " " + channel + " :You're not on that channel")
# define ERR_USERONCHANNEL(user, target, channel) (":" SERVER_NAME " 443 " + user.getNickname() + " " + target + " " + channel + " :is already on channel")
# define ERR_NEEDMOREPARAMS(user, command) (":" SERVER_NAME " 461 " + (user.getNickname().empty() ? "*" : user.getNickname()) + " " + command + " :Not enough parameters")
# define ERR_ALREADYREGISTRED(user) (":" SERVER_NAME " 462 " + (user.getNickname().empty() ? "*" : user.getNickname()) + " :Unauthorized command (already registered)")
# define ERR_CHANNELISFULL(user, channel) (":" SERVER_NAME " 471 " + user.getNickname() + " " + channel + " :Cannot join channel (+l)")
# define ERR_UNKNOWNMODE(user, mode, channel) (":" SERVER_NAME " 472 " + user.getNickname() + " " + mode + " :is unknown mode char to me for " + channel)
# define ERR_INVITEONLYCHAN(user, channel) (":" SERVER_NAME " 473 " + user.getNickname() + " " + channel + " :Cannot join channel (+i)")
# define ERR_BADCHANNELKEY(user, channel) (":" SERVER_NAME " 475 " + user.getNickname() + " " + channel + " :Cannot join channel (+k)")
# define ERR_BADCHANMASK(user, channel) (":" SERVER_NAME " 476 " + user.getNickname() + " " + channel + " :Bad Channel Mask")
# define ERR_CHANOPRIVSNEEDED(user, channel) (":" SERVER_NAME " 482 " + user.getNickname() + " " + channel + " :You're not channel operator")
// custom replies 
# define ERR_ERRONEUSCHANNELNAME(user, channel) (":" SERVER_NAME " 480 " + user.getNickname() + " " + channel + " :Erroneus channel name")
# define ERR_ERRONEUSCHANNELKEY(user, channel) (":" SERVER_NAME " 479 " + user.getNickname() + " " + channel + " :Erroneus channel key")
// other custom errors
# define ERR_GENERAL(user, error) (":" SERVER_NAME " 700 " + user.getNickname() + " " + error)
# define ERR_GENERAL_CHANNEL(user, channel, error) (":" SERVER_NAME " 701 " + user.getNickname() + " " + channel + " " + error)

#endif 