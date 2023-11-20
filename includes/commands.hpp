/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tilmanfs <tilmanfs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:45:05 by tfriedri          #+#    #+#             */
/*   Updated: 2023/11/20 23:56:15 by tilmanfs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "Server.hpp"
# include "Message.hpp"
# include "User.hpp"
# include "num_replies.hpp"
# include <string>

// forward declaration
class User;

// command functions
void    pass(Message &msg, User &usr);
void    nick(Message &msg, User &usr);
void    user(Message &msg, User &usr);
void	join(Message &msg, User &usr);
void	ping(Message &msg, User &usr);
void	privmsg(Message &msg, User &usr);
void	notice(Message &msg, User &usr);
void	part(Message &msg, User &usr);
void	topic(Message &msg, User &usr);
void	kick(Message &msg, User &usr);
void	names(Message &msg, User &usr);
void	mode(Message &msg, User &usr);
void    invite(Message &msg, User &usr);
void    list(Message &msg, User &usr);
void	quit(User &usr);

#endif

