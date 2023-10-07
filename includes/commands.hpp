/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:45:05 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/05 15:16:18 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "Server.hpp"
# include "Message.hpp"
# include "User.hpp"
# include "num_replies.hpp"

class User;

// PASS
void    pass(Message &msg, User &usr);
void    nick(Message &msg, User &usr);
void    user(Message &msg, User &usr);
void	quit(Message &msg, User &usr);
void	join(Message &msg, User &usr);



#endif

