/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 14:17:26 by tfriedri          #+#    #+#             */
/*   Updated: 2023/09/30 14:44:12 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

void    printMessage(const Message msg)
{
    std::cout << "Message:" << std::endl;
    std::cout << "    prefix: " << msg.getPrefix() << std::endl;
    std::cout << "    command: " << msg.getCommand() << std::endl;
    std::cout << "    params: ";
    for (size_t i = 0; i < msg.getParams().size(); i++)
    {
        std::cout << msg.getParams()[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "    trailing: " << msg.getTrailing() << std::endl << std::endl;
}
