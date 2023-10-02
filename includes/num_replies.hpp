/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   num_replies.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 13:59:06 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/02 14:27:09 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUM_REPLIES_HPP
# define NUM_REPLIES_HPP
# include "Client.hpp"

# define RPL_WELCOME(client) (":" SERVER_NAME " 001 " + client.getNickname() + " :Welcome to the Internet Relay Network " + client.getUserIdent() + "\r\n")


#endif