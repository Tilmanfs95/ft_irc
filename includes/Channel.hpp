/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 00:12:48 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/07 00:36:03 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include <vector>
#include "Server.hpp"
#include "Message.hpp"
#include "User.hpp"

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
    bool    k; // key
    
public:
    // user list:
    std::vector<std::string> users;
    
    Channel(/* args */);
    Channel(std::string name, std::string key);
    ~Channel();

    std::string getName() const;
    // add a user to the channel or throw an exception
    void        addUser(User &usr, const std::string key);
    
    void        setAsOperator(std::string nickname);
    void        removeAsOperator(std::string nickname);

    // function to send a message to all users of the channel
    void        sendMessage(const Message &msg);
};

#endif
