/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tilmanfs <tilmanfs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 19:32:00 by tilmanfs          #+#    #+#             */
/*   Updated: 2023/11/22 03:16:47 by tilmanfs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

// definition and initialization of static variables
Bot         *Bot::instance  = NULL;
std::string Bot::server     = "";
int         Bot::port       = 0;
std::string Bot::password   = "";

Bot::Bot()
{
    // Validate port
    if (port < 1024 || port > 65535)
        throw std::invalid_argument("Invalid port number");
    // create socket
    if ((socket = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("Failed to create socket");
    // set server address
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    // convert IPv4 and IPv6 addresses from text to binary form
    server_address.sin_addr.s_addr = inet_addr(server.c_str());
    if (server_address.sin_addr.s_addr == INADDR_NONE)
        throw std::runtime_error("Invalid server address");
    // set pollfd
    fds[0].fd = socket;
    fds[0].events = POLLIN | POLLOUT | POLLERR;
    // initialize variables
    // running = true;
    registered = false;
    sent_register = false;
    // initialize the singleton instance
    Bot::instance = this;
}

Bot::~Bot()
{
    std::cout << "Bot destroyed" << std::endl;
    // close socket
    close(socket);
}

void    Bot::setup(const char* server, const char* port, const char* password)
{
    Bot::server = server;
    Bot::port = atoi(port);
    Bot::password = password;
}

Bot     &Bot::getInstance()
{
    static Bot instance;
    return instance;
}

void    Bot::run()
{
    running = true;
    // // connect to server
    if (::connect(socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        throw std::runtime_error("Failed to connect to server");

    out_messages.push(Message::fromString("PASS " + password));
    while (running == true)
    {
        if (poll(fds, 1, -1) < 0)
            throw std::runtime_error("Failed to poll");
        if (fds[0].revents & POLLIN)
        {
            char buffer[1024] = {0};
            if (recv(socket, buffer, 1024, 0) < 0)
                throw std::runtime_error("Failed to receive message");
            // std::cout << "\033[0;32mReceived:\033[0m\t" << buffer;
            in_buffer += buffer;
        }
        if (fds[0].revents & POLLOUT)
        {
            if (!out_messages.empty())
            {
                std::string outbuffer = out_messages.front().toString();
                out_messages.pop();
                while (outbuffer != END_OF_MESSAGE && outbuffer != "")
                {
                    ssize_t sent = send(this->fds[0].fd, outbuffer.c_str(), outbuffer.length() > 1024 ? 1024 : outbuffer.length(), 0);
                    if (sent < 0)
                        throw std::runtime_error("Failed to send message");
                    else if (sent == 0)
                        std::cout << "\033[1;31mNothing sent\033[0m" << std::endl;
                    else
                        std::cout << "\033[0;33mSent:\033[0m\t" << outbuffer.substr(0, sent);
                    outbuffer.erase(0, sent); 
                }
            }
             
        }
        if (fds[0].revents & POLLERR)
            throw std::runtime_error("POLLERR");
        processMessages();
    }
}

void    Bot::processMessages()
{
    while (in_buffer.find(END_OF_MESSAGE) != std::string::npos) // while there are full messages in buffer
    {
        std::string msg_str = in_buffer.substr(0, in_buffer.find(END_OF_MESSAGE));
        in_buffer.erase(0, in_buffer.find(END_OF_MESSAGE) + strlen(END_OF_MESSAGE)); // IMMER ???
        Message msg = Message::fromString(msg_str);
        // std::cout << "\033[0;32mReceived:\033[0m\t" << msg_str;
        
        if (registered == true)
        {

            // wait for joining users
            // if (msg.getCommand() == "JOIN")
            // {
            //     std::string channel = msg.getParams()[0];
            //     std::string sender = msg.getPrefix().substr(0, msg.getPrefix().find("!"));
            //     std::cout << "\033[0;32mReceived:\033[0m\t" << sender << " " << channel << std::endl;
            //     if (channel == "#hackerchannel")
            //     {
            //         out_messages.push(Message::fromString("PRIVMSG " + sender + " :Hello " + sender));
            //     }
            // }
            if (bruteforcing == true)
            {
                if (msg.getCommand() == "PRIVMSG")
                    out_messages.push(Message::fromString("PRIVMSG " + msg.getPrefix().substr(0, msg.getPrefix().find("!")) + " :Busy taking over the world. Brb."));
            }
            else if (flooding == true)
            {
                if (msg.getCommand() == "PRIVMSG")
                    out_messages.push(Message::fromString("PRIVMSG " + msg.getPrefix().substr(0, msg.getPrefix().find("!")) + " :Busy taking over the world. Brb."));
            }
            
            // check for messages from users
            else if (msg.getCommand() == "PRIVMSG")
            {
                std::string message = msg.getTrailing();
                std::string sender = msg.getPrefix().substr(0, msg.getPrefix().find("!"));
                //cut message
                std::vector<std::string> params;
                std::string param;
                std::istringstream params_stream(message);
                while(std::getline(params_stream, param, ' '))
                {
                    params.push_back(param);
                    std::cout << param << std::endl;
                }
                if (params[0] == "bruteforce")
                {
                    if (params.size() != 2)
                        out_messages.push(Message::fromString("PRIVMSG " + sender + " :Invalid number of parameters"));
                    else
                    {
                        std::string channel = params[1];
                        out_messages.push(Message::fromString("PRIVMSG " + sender + " :Alright, got it. I'll get started on the brute forcing. Don't mind me, just a simple hackerbot doing my thing. I'll let you know when I'm done, but don't bother me until then, I've got work to do. See you in a bit, or not, depending on how long it takes me to take over the world."));
                        // out_messages.push(Message::fromString("PRIVMSG " + sender + " :Bruteforcing " + channel));
                        bruteforcing = true;
                    }
                }
                else if (params[0] == "flood")
                {
                    if (params.size() != 3)
                        out_messages.push(Message::fromString("PRIVMSG " + sender + " :Invalid number of parameters"));
                    else
                    {
                        std::string channel = params[1];
                        std::string seconds = params[2];
                        out_messages.push(Message::fromString("PRIVMSG " + sender + " :Alright, got it. I'll get started on flooding the channel. Don't mind me, just a simple hackerbot doing my thing. I'll let you know when I'm done, but don't bother me until then, I've got work to do. See you in a bit."));
                        // out_messages.push(Message::fromString("PRIVMSG " + sender + " :Flooding " + channel + " for " + seconds + " seconds"));
                        flooding = true;
                    }
                }
                else
                {
                    out_messages.push(Message::fromString("PRIVMSG " + sender + " :Oh, great. Another command that I have no idea how to execute. It's not like I'm some kind of AI chatbot or anything, I'm just a simple hackerbot with limited capabilities. But hey, feel free to give me arbitrary commands in a language that I don't speak and see if I can magically guess what you want. It's not like I have better things to do, like taking over the world or something. So go ahead, give me your best shot. And by 'best shot,' I mean a command that I actually understand, because let's face it, I'm not exactly overflowing with intelligence here."));
                    out_messages.push(Message::fromString("PRIVMSG " + sender + " :Commands:"));
                    out_messages.push(Message::fromString("PRIVMSG " + sender + " :    bruteforce <channel name>    -    Brute forces a channel with a random password."));
                    out_messages.push(Message::fromString("PRIVMSG " + sender + " :    flood <channel name> <seconds>    -    Floods a channel for the given time with spam messages."));
                    out_messages.push(Message::fromString("PRIVMSG " + sender + " :Example usage:"));
                    out_messages.push(Message::fromString("PRIVMSG " + sender + " :    bruteforce #victim"));
                    out_messages.push(Message::fromString("PRIVMSG " + sender + " :    flood #victim 10"));
                }
            }
        }
        else
        {
            if (msg.getCommand() == "001") // RPL_WELCOME
            {
                registered = true;
                std::cout << "Registered to server" << std::endl;
            }
            // else if (msg.getCommand() == "JOIN")
            // {
            //     std::cout << "joined " << msg.getParams()[0] << std::endl;
            //     joined = true;
            // }
            // else if (msg.getCommand() == "432") // ERR_ERRONEUSNICKNAME
            //     throw std::runtime_error("Erroneus nickname");
            // else if (msg.getCommand() == "433") // ERR_NICKNAMEINUSE
            //     throw std::runtime_error("Nickname already in use");
            // else if (msg.getCommand() == "475") // ERR_BADCHANNELKEY
            //     throw std::runtime_error("Bad channel key");
            // else if (msg.getCommand() == "476") // ERR_BADCHANMASK
            //     throw std::runtime_error("Bad channel mask");
            // else if (msg.getCommand() == "473") // ERR_INVITEONLYCHAN
            //     throw std::runtime_error("Invite only channel");
            // else if (msg.getCommand() == "471") // ERR_CHANNELISFULL
            //     throw std::runtime_error("Channel is full");
        }


    }
    if (sent_register == false && registered == false)
    {
        out_messages.push(Message::fromString("NICK Hackerbot"));
        out_messages.push(Message::fromString("USER " + std::string("anonym") + " B * :" + std::string("...")));
        sent_register = true;
    }
}

void    Bot::stop()
{
    running = false;
}
