/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tilmanfs <tilmanfs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 14:10:41 by tfriedri          #+#    #+#             */
/*   Updated: 2023/11/20 23:55:45 by tilmanfs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"

// easy implementation of the QUIT command without any parameters

void	quit(User &usr)
{
	Server	*server = &Server::getInstance();
	server->removeUser(usr.getSocket());
}
