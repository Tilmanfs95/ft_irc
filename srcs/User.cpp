/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 18:40:50 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/11 00:50:31 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/User.hpp"

User::User()
{
}

User::User(int socket, std::string host_ip) : socket(socket), host_ip(host_ip)
{
    this->verified = false;
    this->registered = false;
    this->mode = 0;
    this->nickname = "";
    this->username = "";
    this->realname = "";
    this->channels = std::vector<std::string>();
    this->out_messages = std::queue<Message>();
    this->in_buffer = std::string();
}

User::~User()
{
}


// Setters

void    User::setRegistered(bool registered)
{
    this->registered = registered;
}

void	User::setVerified(bool verified)
{
    this->verified = verified;
}

void    User::setMode(int mode)
{
    this->mode = mode;
}

void    User::setNickname(const std::string &nickname)
{
	this->nickname = nickname;
}

void    User::setUsername(const std::string &username)
{
    this->username = username;
}

void    User::setRealname(const std::string &realname)
{
    this->realname = realname;
}

void    User::addChannel(const std::string &channel)
{
    this->channels.push_back(channel);
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

std::string User::getHostIp() const
{
    return this->host_ip;
}

std::string User::getUserIdent() const
{
    return this->nickname + "!" + this->username + "@" + this->host_ip;
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