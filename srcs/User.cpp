/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 18:40:50 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/04 21:17:15 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/User.hpp"

User::User()
{
}

User::User(int socket) : socket(socket)
{
    this->verified = false;
    this->registered = false;
    this->mode = 0;
    this->nickname = "";
    this->username = "";
    this->realname = "";
    this->host_ip = "";
}

User::User(int socket, std::string host_ip) : socket(socket), host_ip(host_ip)
{
    this->verified = false;
    this->registered = false;
    this->mode = 0;
    this->nickname = "";
    this->username = "";
    this->realname = "";
}

User::~User()
{
}


// Setters

void    User::setRegistered(bool registered)
{
    this->registered = registered;
}

void    User::setNickname(const std::string &nickname)
{
	this->nickname = nickname;
}


// Getters

int		User::getSocket() const
{
    return this->socket;
}

bool	User::getVerified() const
{
    return this->verified;
}

bool	User::getRegistered() const
{
    return this->registered;
}

std::string User::getNickname() const
{
    return this->nickname;
}

std::string User::getUsername() const
{
    return this->username;
}

std::string User::getRealname() const
{
    return this->realname;
}

std::string User::getUserIdent() const
{
    return this->nickname + "!~" + this->username + "@" + this->host_ip;
}


// Setters

void	User::setVerified(bool verified)
{
    this->verified = verified;
}

void        User::processInput(const std::string &msg)
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

void		User::addOutMessage(const Message &msg)
{
    this->out_messages.push(msg);
}

Message     User::getOutMessage()
{
    if (this->out_messages.empty())
        throw std::runtime_error("No message to send");
    Message msg = this->out_messages.front();
    this->out_messages.pop();
    return msg;
}