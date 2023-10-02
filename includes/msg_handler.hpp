/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:45:05 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/02 15:13:39 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HANDLER_HPP
# define MSG_HANDLER_HPP

# include "Server.hpp"
# include "Message.hpp"
# include "Client.hpp"
# include "num_replies.hpp"

class Client;

void	handleMessage(Message &msg, Client &usr);

#endif

