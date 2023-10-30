/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 00:12:48 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/30 13:23:10 by tfriedri         ###   ########.fr       */
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
    std::string name;
    // channel key:
    // -case sensitive
    std::string key;
    // channel topic:  (empty string if no topic is set)
    std::string topic;
    // user limit:
    unsigned int limit;

public:
    // user list (incl. operators)
    std::vector<std::string> users;
	// operator list:
    std::vector<std::string> operators;
    // invite list:
    std::vector<std::string> invites;
    // channel modes:
    bool    i; // invite only (only invited users can join)
    bool    t; // topic restricted (only operators can change the topic)
	bool    k; // key required (users need to know the key to join)
	bool    l; // user limit set (only a certain number of users can join)
    
	// constructors and destructor
    Channel();
    Channel(std::string name, std::string key);
    ~Channel();

	// Setters
	
	// sets the topic of the channel
	void        setTopic(const std::string &topic);
	// sets the key of the channel
	// important: you also need to set the k mode to true!
	void        setKey(const std::string &key);
	// sets the limit of the channel
	// important: you also need to set the l mode to true!
	void        setLimit(unsigned int limit);
	
	// Getters
	
    // gets the name of the channel
    std::string getName() const;
	// gets the topic of the channel
	std::string getTopic() const;
	// gets the key of the channel
	std::string getKey() const;
	// gets the limit of the channel
	unsigned int getLimit() const;

	// Methods
	
	// gets the channel modes as a string.
	// depending on the user requesting the modes (if the user is in the channel),
	// the output includes the channel key and the user limit as parameters
	std::string getModes(User &requester) const;
    // adds a user to the channel
    // (also sends a message to the user and all other users of the channel)
    void        addUser(User &usr, const std::string key, bool isOperator);
    // remove a user from the channel
    // (does not send a message to the user itself but to all other users of the channel)
    // you need to check if the channel is empty afterwards and delete it if it is
    void        removeUser(User &usr, std::string partMessage, bool sendMessages = true);
	// checks if the user is in the channel
	// the argument is the nickname of the user (case sensitive !)
	bool        isUser(const std::string &nickname) const;
	// checks if the user is an operator of the channel
	// the argument is the nickname of the user (case sensitive !)
	bool        isOperator(const std::string &nickname) const;
    // sends a message to all users of the channel (except the sender)
	// if sender is not specified, the message is sent to all users
    void        sendMessage(const Message &msg, const std::string &sender = "");
};

#endif
