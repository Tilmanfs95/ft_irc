/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:45:05 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/04 15:51:39 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HANDLER_HPP
# define MSG_HANDLER_HPP

# include "Server.hpp"
# include "Message.hpp"
# include "User.hpp"
# include "num_replies.hpp"

class User;

// message handler
void	handleMessage(Message &msg, User &usr);

// PASS
void    pass(Message &msg, User &usr);
void    nick(Message &msg, User &usr);



#endif

