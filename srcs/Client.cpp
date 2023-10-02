/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 18:40:50 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/02 15:13:44 by tfriedri         ###   ########.fr       */
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


// Setters

// std::string Client::setNickname(const std::string &nickname)
// {
// 	if (nickname.length() > 9)
// 		throw std::invalid_argument("Nickname too long");
// 	this->nickname = nickname;
// 	return this->nickname;
// }


// Getters

int		Client::getSocket() const
{
	return this->socket;
}

bool	Client::getVerified() const
{
	return this->verified;
}

bool	Client::getRegistered() const
{
	return this->registered;
}

std::string Client::getNickname() const
{
    return this->nickname;
}

std::string Client::getUserIdent() const
{
	return this->username + "@" + this->hostname;
}


// Setters

void	Client::setVerified(bool verified)
{
	this->verified = verified;
}

void        Client::processInput(const std::string &msg)
{
    // std::cout << "Socket " << this->socket << ": " << msg << std::endl;
    this->in_buffer.append(msg);
    // process messagebuffer:
    while (this->in_buffer.find(END_OF_MESSAGE) != std::string::npos)
    {
        std::string msg_str = this->in_buffer.substr(0, this->in_buffer.find(END_OF_MESSAGE));
        this->in_buffer.erase(0, this->in_buffer.find(END_OF_MESSAGE) + strlen(END_OF_MESSAGE));
        Message msg = Message::fromString(msg_str);

		std::cout << "\033[35mSocket " << this->socket << ":\033[0m ";
        printMessage(msg);
		handleMessage(msg, *this);
    }
}

void		Client::addOutMessage(const Message &msg)
{
	this->out_messages.push(msg);
}

Message     Client::getOutMessage()
{
    if (this->out_messages.empty())
        throw std::runtime_error("No message to send");
    Message msg = this->out_messages.front();
    this->out_messages.pop();
    return msg;
}
