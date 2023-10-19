#include "../includes/Channel.hpp"

Channel::Channel(/* args */)
{
}

Channel::Channel(std::string name, std::string key): name(name), key(key)
{
	this->topic = "";
	this->operators = std::vector<std::string>();
	this->users = std::vector<std::string>();
	this->invites = std::vector<std::string>();
	this->limit = 0;
	this->i = false;
	this->t = false;
	if (key.empty())
		this->k = false;
	else
		this->k = true;
	this->l = false;
	if (key.empty())
		std::cout << "Created channel " << name << " with no key" << std::endl;
	else
		std::cout << "Created channel " << name << " with key " << key << std::endl;
}

Channel::~Channel()
{
	std::cout << "Channel " << this->name << " closed" << std::endl;
}

std::string		Channel::getName() const
{
	return (this->name);
}

std::string		Channel::getTopic() const
{
	return (this->topic);
}

void			Channel::setTopic(const std::string &topic)
{
	this->topic = topic;
}

std::string		Channel::getKey() const
{
	return (this->key);
}

void			Channel::setKey(const std::string &key)
{
	this->key = key;
}

unsigned int	Channel::getLimit() const
{
	return (this->limit);
}

void			Channel::setLimit(unsigned int limit)
{
	this->limit = limit;
}

std::string	Channel::getModes(User &requester) const
{
	std::string modes = "+";
	std::string params;
	if (this->i)
		modes += "i";
	if (this->t)
		modes += "t";
	if (this->k)
	{
		modes += "k";
		params += " " + this->key;
	}
	if (this->l)
	{
		modes += "l";
		params += " " + std::to_string(this->limit);
	}
	if (std::find(this->users.begin(), this->users.end(), requester.getNickname()) != this->users.end())
		return (modes + params);	
	return (modes);
}

void			Channel::addUser(User &usr, std::string key, bool isOperator)
{
	// check if user is already in channel
	if (std::find(this->users.begin(), this->users.end(), usr.getNickname()) != this->users.end())
		return ;
	// check if user is allowed to join
	if (this->key != key)
		usr.addOutMessage(Message::fromString(ERR_BADCHANNELKEY(usr, this->name)));
	// check if channel is full
	else if (this->l && this->users.size() >= this->limit)
		usr.addOutMessage(Message::fromString(ERR_CHANNELISFULL(usr, this->name)));
	// check if channel is invite only and user is invited
	else if (this->i && std::find(this->invites.begin(), this->invites.end(), usr.getNickname()) == this->invites.end())
		usr.addOutMessage(Message::fromString(ERR_INVITEONLYCHAN(usr, this->name)));
	else
	{
		// add user to channel
		this->users.push_back(usr.getNickname());
		// if user is operator, add user to operators list
		if (isOperator)
			this->operators.push_back(usr.getNickname());
		// add channel to the users list of joined channels
		usr.addChannel(this->name);
		// send JOIN message to all users in channel
		sendMessage(Message::fromString(":" + usr.getUserIdent() + " JOIN " + this->name));
		// send all channel messages to the user
		usr.in_buffer += "TOPIC " + this->name + END_OF_MESSAGE;
		usr.in_buffer += "NAMES " + this->name + END_OF_MESSAGE;
		// usr.in_buffer += "MODE " + this->name + END_OF_MESSAGE;
	}
	// print users channel list
	// std::cout << usr.getNickname() << " is in channels: ";
	// for (size_t i = 0; i < usr.channels.size(); i++)
	// 	std::cout << usr.channels[i] << " ";
	// std::cout << std::endl;
}

void			Channel::removeUser(User &usr, std::string partMessage, bool sendMessages)
{
	// check if user is in channel
	if (std::find(this->users.begin(), this->users.end(), usr.getNickname()) == this->users.end())
		return ;
	// remove user from channel
	if (std::find(this->users.begin(), this->users.end(), usr.getNickname()) != this->users.end())
		this->users.erase(std::find(this->users.begin(), this->users.end(), usr.getNickname()));
	// remove user from operators list
	if (std::find(this->operators.begin(), this->operators.end(), usr.getNickname()) != this->operators.end())
		this->operators.erase(std::find(this->operators.begin(), this->operators.end(), usr.getNickname()));
	// if channel is not empty but user was the last operator, make the next user operator
	if (!this->users.empty() && this->operators.empty())
	{
		this->operators.push_back(this->users[0]);
		sendMessage(Message::fromString(":" + usr.getUserIdent() + " MODE " + this->name + " +o " + this->users[0]));
	}
	// remove channel from the users list of joined channels
	if (std::find(usr.channels.begin(), usr.channels.end(), this->name) != usr.channels.end())
		usr.channels.erase(std::find(usr.channels.begin(), usr.channels.end(), this->name));
	// send PART message to all users in channel
	if (sendMessages == true)
	{
		if (partMessage.empty())
			sendMessage(Message::fromString(":" + usr.getUserIdent() + " PART " + this->name));
		else
			sendMessage(Message::fromString(":" + usr.getUserIdent() + " PART " + this->name + " :" + partMessage));
	}
	std::cout << "Removed " << usr.getNickname() << " from " << this->name << std::endl;
}

bool			Channel::isUser(const std::string &nickname) const
{
	if (std::find(this->users.begin(), this->users.end(), nickname) != this->users.end())
		return (true);
	return (false);
}

bool			Channel::isOperator(const std::string &nickname) const
{
	if (std::find(this->operators.begin(), this->operators.end(), nickname) != this->operators.end())
		return (true);
	return (false);
}

void			Channel::sendMessage(const Message &msg, const std::string &sender)
{
	std::string	sender_upper = sender;
	std::transform(sender_upper.begin(), sender_upper.end(), sender_upper.begin(), ::toupper);
	for (size_t i = 0; i < this->users.size(); i++)
	{
		std::string user_upper = this->users[i];
		std::transform(user_upper.begin(), user_upper.end(), user_upper.begin(), ::toupper);
		int socket = server->nick_to_sock[user_upper];
		if (user_upper != sender_upper)
			server->users[socket].addOutMessage(msg);
	}
}
