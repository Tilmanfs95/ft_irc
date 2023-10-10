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
	this->t = true;
	if (key.empty())
		std::cout << "Created channel " << name << " with no key" << std::endl;
	else
		std::cout << "Created channel " << name << " with key " << key << std::endl;
}

Channel::~Channel()
{
}

std::string		Channel::getName() const
{
	return (this->name);
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
	else if (this->limit > 0 && this->users.size() >= this->limit)
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
		// send RPL_TOPIC or RPL_NOTOPIC to the joining user
		if (this->topic.empty())
			usr.addOutMessage(Message::fromString(RPL_NOTOPIC(usr, this->name)));
		else
			usr.addOutMessage(Message::fromString(RPL_TOPIC(usr, this->name, this->topic)));
		// send RPL_NAMREPLY to the joining user
		std::string users = "";
		for (size_t i = 0; i < this->users.size(); i++)
		{
			if (std::find(this->operators.begin(), this->operators.end(), this->users[i]) != this->operators.end())
				users += "@";
			users += this->users[i];
			if (i < this->users.size() - 1)
				users += " ";
		}
		usr.addOutMessage(Message::fromString(RPL_NAMREPLY(usr, this->name, users)));
		// send RPL_ENDOFNAMES to the joining user
		usr.addOutMessage(Message::fromString(RPL_ENDOFNAMES(usr, this->name)));
	}
	// print users channel list
	// std::cout << usr.getNickname() << " is in channels: ";
	// for (size_t i = 0; i < usr.channels.size(); i++)
	// 	std::cout << usr.channels[i] << " ";
	// std::cout << std::endl;
}

void			Channel::removeUser(User &usr, std::string partMessage)
{
	// check if user is in channel
	if (std::find(this->users.begin(), this->users.end(), usr.getNickname()) == this->users.end())
		return ;
	// remove user from channel
	this->users.erase(std::find(this->users.begin(), this->users.end(), usr.getNickname()));
	// remove user from operators list
	this->operators.erase(std::find(this->operators.begin(), this->operators.end(), usr.getNickname()));
	// remove channel from the users list of joined channels
	usr.channels.erase(std::find(usr.channels.begin(), usr.channels.end(), this->name));
	// send PART message to all users in channel
	if (partMessage.empty())
		sendMessage(Message::fromString(":" + usr.getUserIdent() + " PART " + this->name));
	else
		sendMessage(Message::fromString(":" + usr.getUserIdent() + " PART " + this->name + " :" + partMessage));
	//
	std::cout << "Removed " << usr.getNickname() << " from " << this->name << std::endl;
}

// void			Channel::setAsOperator(std::string nickname)
// {
// 	if (std::find(this->users.begin(), this->users.end(), nickname) == this->users.end())
// 		throw std::invalid_argument("User is not in channel");
// 	if (std::find(this->operators.begin(), this->operators.end(), nickname) != this->operators.end())
// 		throw std::invalid_argument("User is already operator");
// 	this->operators.push_back(nickname);
// }

// void			Channel::removeAsOperator(std::string nickname)
// {
// 	if (std::find(this->users.begin(), this->users.end(), nickname) == this->users.end())
// 		throw std::invalid_argument("User is not in channel");
// 	if (std::find(this->operators.begin(), this->operators.end(), nickname) == this->operators.end())
// 		throw std::invalid_argument("User is not operator");
// 	this->operators.erase(std::find(this->operators.begin(), this->operators.end(), nickname));
// }

void			Channel::sendMessage(const Message &msg)
{
	for (size_t i = 0; i < this->users.size(); i++)
	{
		int socket = server->nick_to_sock[this->users[i]];
		server->users[socket].addOutMessage(msg);
	}
}
