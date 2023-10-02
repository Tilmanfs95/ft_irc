/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 14:17:26 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/01 22:57:04 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Message.hpp"

void    printMessage(const Message msg)
{
    // std::cout << "Message: ";
    std::cout << "prefix: " << msg.getPrefix() << "\tcommand: " << msg.getCommand();
    std::cout << "\tparams: ";
    for (size_t i = 0; i < msg.getParams().size(); i++)
        std::cout << msg.getParams()[i] << " ";
    std::cout << "\ttrailing: " << msg.getTrailing() << std::endl;
}
