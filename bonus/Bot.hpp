/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tilmanfs <tilmanfs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 19:24:29 by tilmanfs          #+#    #+#             */
/*   Updated: 2023/11/22 04:32:33 by tilmanfs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

#include "../includes/Message.hpp"
#include "../includes/defines.hpp"
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <netinet/in.h>
#include <poll.h>
#include <queue>

class Bot
{
  private:
    // singleton instance
    static Bot                      *instance;
    // private constructor & destructor for singleton
    Bot();
    ~Bot();
    static std::string              server;
    static int                      port;
    static std::string              password;
    int                             socket;
    struct sockaddr_in              server_address;
    struct pollfd                   fds[1];
    bool                            running;
    bool                            registered;
    bool                            sent_register;
    //
    bool                            bruteforcing;
    bool                            waiting_for_response;
    bool                            flooding;
    std::string                     customer;
    std::string                     victim;
    //
    std::queue<Message>             out_messages;
    std::string                     in_buffer;
    //
    void                            sendMsg(Message msg);
    void                            floodChannel();
    void                            processMessages();
  public:
    static void setup(const char* server, const char* port, const char* password);
    static Bot  &getInstance();
    void        run();
    void        stop();
};

#endif