/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 18:40:50 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/02 10:35:32 by tfriedri         ###   ########.fr       */
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
    // std::cout << "Socket " << this->socket << ": " << msg << std::endl;
    this->in_buffer.append(msg);
    // process messagebuffer:
    while (this->in_buffer.find(END_OF_MESSAGE) != std::string::npos)
    {
        std::string msg_str = this->in_buffer.substr(0, this->in_buffer.find(END_OF_MESSAGE));
        this->in_buffer.erase(0, this->in_buffer.find(END_OF_MESSAGE) + strlen(END_OF_MESSAGE));
        Message msg = Message::fromString(msg_str);

        // print message for debugging
		// std::cout << "Socket " << this->socket << ": ";
		// in purple
		std::cout << "\033[35mSocket " << this->socket << ":\033[0m ";
        printMessage(msg);
        if (msg.getCommand() == "CAP") // do not process CAP messages
            continue;
        if (this->verified == false)
        {
            // only allow PASS command
            if (msg.getCommand() == "PASS" && msg.getParams().size() == 1)
            {
                if (msg.getParams()[0] == server->getPassword())
                {
                    std::cout << "Password correct" << std::endl;
                    this->verified = true;
                    
                    // following message makes no sense and is only for testing purposes
                    this->out_messages.push(Message::fromString(":" + server->getName() + " 001 " + this->nickname + " :Welcome to the Internet Relay Network " + this->nickname + "!" + this->username + "@" + this->hostname + END_OF_MESSAGE));
                }
                else
                {
                    std::cout << "Password incorrect" << std::endl;
                    //
                }
            }
        }
        else if (this->registered == false)
        {
            // only allow NICK and USER commands
        }
        else
        {
            // process message
        }
        
    }
}

std::string Client::getNickname() const
{
    return this->nickname;
}

Message     Client::getOutMessage()
{
    if (this->out_messages.empty())
        throw std::runtime_error("No message to send");
    Message msg = this->out_messages.front();
    this->out_messages.pop();
    return msg;
}
