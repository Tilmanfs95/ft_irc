/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 19:24:29 by tilmanfs          #+#    #+#             */
/*   Updated: 2023/11/23 14:42:56 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

#include "../includes/Message.hpp"
#include "../includes/defines.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <signal.h>
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
    static std::string              nick;
    int                             socket;
    struct sockaddr_in              server_address;
    struct pollfd                   fds[1];
    bool                            running;
    bool                            registered;
    bool                            sent_register;
    //
    bool                            bruteforcing;
    std::ifstream                   wordlist;
    std::string                     word;
    bool                            waiting_for_response;
    bool                            flooding;
    std::string                     customer;
    std::string                     victim;
    //
    std::queue<Message>             out_messages;
    std::string                     in_buffer;
    //
    void                            sendMsg(Message msg);
    void                            runAttacks();
	std::string                     generateRandomMessage();
	void							checkMessages(Message &msg);
    void                            work();
  public:
    static void setup(const char* server, const char* port, const char* password, const char* nickname);
    static Bot  &getInstance();
    void        run();
    void        stop();
};

#endif