/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 18:40:43 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/30 13:13:37 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Message.hpp"

Message::Message()
{
}

Message::~Message()
{
}


// Setters

void    Message::setPrefix(const std::string &prefix) {
    this->prefix = prefix;
}

void    Message::setCommand(const std::string &command) {
    this->command = command;
}

void    Message::setParams(const std::vector<std::string> &params) {
    this->params = params;
}

void    Message::setTrailing(const std::string &trailing) {
    this->trailing = trailing;
}


// Getters

std::string Message::getPrefix() const {
    return this->prefix;
}

std::string Message::getCommand() const {
    return this->command;
}

std::vector<std::string> Message::getParams() const {
    return this->params;
}

bool	Message::trailingExists() const {
	return this->trailing_exists;
}

std::string Message::getTrailing() const {
    return this->trailing;
}


// Methods

Message  Message::fromString(const std::string &msg) {
    Message message;
    std::istringstream iss(msg);
    std::string token;

    // Prefix (if present)
    if (msg[0] == ':') {
        iss >> token;
        message.setPrefix(token.substr(1)); // removes ':'
    }
    // Command
    iss >> token;
    std::transform(token.begin(), token.end(), token.begin(), ::toupper);
    message.setCommand(token);
	message.trailing_exists = false;
    while (iss >> token)
    {
        if (token[0] == ':')
        {
            // Trailing part
			message.trailing_exists = true;
            std::string rest = token.substr(1); // removes ':'
            while (iss >> token) {
                rest += " " + token;
            }
            message.setTrailing(rest);
            break;
        } 
        else // Parameters
            message.addParam(token);
    }
    return message;
}

std::string Message::toString() const {
    std::string message;

    // Add the prefix (if present)
    if (!this->prefix.empty()) {
        message += ":" + this->prefix + " ";
    }
    // Add the command
    message += this->command + " ";
    // Add the parameters
    for (size_t i = 0; i < this->params.size(); i++) {
        message += this->params[i] + " ";
    }
    // Add the trailing part (if present)
    if (this->trailingExists())
        message += ":";
    message += this->trailing;
	// Add the END_OF_MESSAGE character
	message += END_OF_MESSAGE;
    
    return message;
}

void    Message::addParam(const std::string &param) {
    this->params.push_back(param);
}

void	Message::delParam(size_t index)
{
	if (index >= this->params.size())
		return ;
	this->params.erase(this->params.begin() + index);
}
