/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   num_replies.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 13:59:06 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/10 21:51:44 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUM_REPLIES_HPP
# define NUM_REPLIES_HPP
# include "User.hpp"


# define RPL_WELCOME(user) (":" SERVER_NAME " 001 " + user.getNickname() + " :Welcome to the Internet Relay Network " + user.getUserIdent())


# define ERR_NEEDMOREPARAMS(user, command) (":" SERVER_NAME " 461 " + (user.getNickname().empty() ? "*" : user.getNickname()) + " " + command + " :Not enough parameters")
# define ERR_ALREADYREGISTRED(user) (":" SERVER_NAME " 462 " + (user.getNickname().empty() ? "*" : user.getNickname()) + " :Unauthorized command (already registered)")

# define ERR_NONICKNAMEGIVEN(user) (":" SERVER_NAME " 431 " + (user.getNickname().empty() ? "*" : user.getNickname()) + " :No nickname given")
# define ERR_ERRONEUSNICKNAME(user, nick) (":" SERVER_NAME " 432 " + (user.getNickname().empty() ? "*" : user.getNickname()) + " " + nick + " :Erroneus nickname")
# define ERR_NICKNAMEINUSE(user, nick) (":" SERVER_NAME " 433 " + (user.getNickname().empty() ? "*" : user.getNickname()) + " " + nick + " :Nickname is already in use")


# define RPL_NOTOPIC(user, channel) (":" SERVER_NAME " 331 " + user.getNickname() + " " + channel + " :No topic is set")
# define RPL_TOPIC(user, channel, topic) (":" SERVER_NAME " 332 " + user.getNickname() + " " + channel + " :" + topic)
# define RPL_NAMREPLY(user, channel, nicknames) (":" SERVER_NAME " 353 " + user.getNickname() + " = " + channel + " :" + nicknames)
# define RPL_ENDOFNAMES(user, channel) (":" SERVER_NAME " 366 " + user.getNickname() + " " + channel + " :End of NAMES list")
# define ERR_BADCHANNELKEY(user, channel) (":" SERVER_NAME " 475 " + user.getNickname() + " " + channel + " :Cannot join channel (+k)")
# define ERR_CHANNELISFULL(user, channel) (":" SERVER_NAME " 471 " + user.getNickname() + " " + channel + " :Cannot join channel (+l)")
# define ERR_INVITEONLYCHAN(user, channel) (":" SERVER_NAME " 473 " + user.getNickname() + " " + channel + " :Cannot join channel (+i)")
# define ERR_BADCHANMASK(user, channel) (":" SERVER_NAME " 476 " + user.getNickname() + " " + channel + " :Bad Channel Mask")


// custom replies
# define ERR_ERRONEUSCHANNELNAME(user, channel) (":" SERVER_NAME " 479 " + user.getNickname() + " " + channel + " :Erroneus channel name")
# define ERR_ERRONEUSCHANNELKEY(user, channel) (":" SERVER_NAME " 479 " + user.getNickname() + " " + channel + " :Erroneus channel key")

#endif 