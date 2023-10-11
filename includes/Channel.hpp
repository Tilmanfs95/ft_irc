/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 00:12:48 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/11 22:11:50 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include <vector>
#include "Server.hpp"
#include "Message.hpp"
#include "User.hpp"
#include "defines.hpp"

class User;

class Channel
{
private:
    // channel name:
    // -max 50 chars
    // -must start with # or &
    // -must not contain ' ' , ',' , ':' , '^G' or ASCII 7
    // -must be unique
    // -not case sensitive
    std::string name;
    // channel key:
    // -case sensitive
    std::string key;
    // channel topic:
    std::string topic;
    // operator list:
    std::vector<std::string> operators;
    // invite list:
    std::vector<std::string> invites;
    // user limit:
    unsigned int limit;
    // modes:
    bool    i; // invite only
    bool    t; // topic restrict
    
public:
    // user list:
    std::vector<std::string> users; // list of all nicknames (incl. operators)
    
    Channel(/* args */);
    Channel(std::string name, std::string key);
    ~Channel();

    // gets the name of the channel
    std::string getName() const;
    // adds a user to the channel
    // (also sends a message to the user and all other users of the channel)
    void        addUser(User &usr, const std::string key, bool isOperator);
    // remove a user from the channel
    // (does not send a message to the user itself but to all other users of the channel)
    // you need to check if the channel is empty afterwards and delete it if it is
    void        removeUser(User &usr, std::string partMessage);
    // sends a message to all users of the channel (except the sender)
	// if sender is not specified, the message is sent to all users
    void        sendMessage(const Message &msg, const std::string &sender = std::string());
};

#endif
