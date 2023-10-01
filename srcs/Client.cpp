/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 18:40:50 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/01 15:24:17 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client()
{
}

Client::Client(int socket)
{
    this->socket = socket;
    this->verified = false;
    this->registered = false;
    this->mode = 0;
    this->nickname = "";
    this->username = "";
    this->realname = "";
    this->hostname = "";
}

Client::~Client()
{
}

void        Client::processInput(const std::string &msg)
{
    std::cout << "Socket " << this->socket << ": " << msg << std::endl;
    this->messagebuffer.append(msg);
    // process messagebuffer:
    while (this->messagebuffer.find(END_OF_MESSAGE) != std::string::npos)
    {
        std::string msg_str = this->messagebuffer.substr(0, this->messagebuffer.find(END_OF_MESSAGE));
        this->messagebuffer.erase(0, this->messagebuffer.find(END_OF_MESSAGE) + 2);
        Message msg = Message::fromString(msg_str);
        // print message for debugging
        printMessage(msg);
    }
}

std::string Client::getNickname() const
{
    return this->nickname;
}
