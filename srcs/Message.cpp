/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 18:40:43 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/01 16:33:07 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Message.hpp"

Message::Message(/* args */)
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

void    Message::addParam(const std::string &param) {
    this->params.push_back(param);
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

std::string Message::getTrailing() const {
    return this->trailing;
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
    if (!this->trailing.empty()) {
        message += ":" + this->trailing;
    }
    
    return message;
}

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
    message.setCommand(token);
    while (iss >> token) {
        if (token[0] == ':') {
            // Trailing part
            std::string rest = token.substr(1); // removes ':'
            while (iss >> token) {
                rest += " " + token;
            }
            message.setTrailing(rest);
            break;
        } else {
            // Parameters
            message.addParam(token);
        }
    }
    // check if it's a valid message
    // NOT SURE IF THIS IS NECESSARY
    // if (message.getCommand().empty()) {
    //     throw std::runtime_error("Invalid IRC message: " + msg);
    // }
    return message;
}