#include "../includes/Channel.hpp"

Channel::Channel(/* args */)
{
}

Channel::Channel(std::string name, std::string key): name(name), key(key)
{
	// check if name is valid
	//if (name.empty())
	//	throw std::invalid_argument("Channel name must not be empty");
	if (name[0] != '#' && name[0] != '&')
		throw std::invalid_argument("Channel name must start with # or &");
	if (name.size() > 50)
		throw std::invalid_argument("Channel name must not exceed 50 characters");
	//if (name.find(' ') != std::string::npos)
	//	throw std::invalid_argument("Channel name must not contain spaces");
	//if (name.find(',') != std::string::npos)
	//	throw std::invalid_argument("Channel name must not contain commas");
	if (name.find(7) != std::string::npos)
		throw std::invalid_argument("Channel name must not contain BEL");
	// check if key is valid
	if (key.size() > 50)
		throw std::invalid_argument("Channel key must not exceed 50 characters");
	//if (key.find(' ') != std::string::npos)
	//	throw std::invalid_argument("Channel key must not contain spaces");
	//if (key.find(',') != std::string::npos)
	//	throw std::invalid_argument("Channel key must not contain commas");
	if (key.find(7) != std::string::npos)
		throw std::invalid_argument("Channel key must not contain BEL");
	//
	this->topic = "";
	this->operators = std::vector<std::string>();
	this->users = std::vector<std::string>();
	this->invites = std::vector<std::string>();
	this->limit = 0;
	this->i = false;
	this->t = true;
	this->k = false;
}

Channel::~Channel()
{
}

std::string		Channel::getName() const
{
	return (this->name);
}

void			Channel::addUser(User &usr, std::string key)
{
	// check if user is allowed to join
	if (this->key != key)
		throw std::invalid_argument("Channel key is incorrect");
	// check if user is already in channel
	if (std::find(this->users.begin(), this->users.end(), usr.getNickname()) != this->users.end())
		throw std::invalid_argument("User is already in channel");
	// check if channel is full
	if (this->limit > 0 && this->users.size() >= this->limit)
		throw std::invalid_argument("Channel is full");
	// check if channel is invite only and user is invited
	if (this->i && std::find(this->invites.begin(), this->invites.end(), usr.getNickname()) == this->invites.end())
		throw std::invalid_argument("Channel is invite only");
	
	// everything checked?!
	
	this->users.push_back(usr.getNickname());
	usr.addChannel(this->name);
	// send JOIN message to all users in channel
	sendMessage(Message::fromString(":" + usr.getUserIdent() + " JOIN " + this->name));
}

void			Channel::setAsOperator(std::string nickname)
{
	if (std::find(this->users.begin(), this->users.end(), nickname) == this->users.end())
		throw std::invalid_argument("User is not in channel");
	if (std::find(this->operators.begin(), this->operators.end(), nickname) != this->operators.end())
		throw std::invalid_argument("User is already operator");
	this->operators.push_back(nickname);
}

void			Channel::removeAsOperator(std::string nickname)
{
	if (std::find(this->users.begin(), this->users.end(), nickname) == this->users.end())
		throw std::invalid_argument("User is not in channel");
	if (std::find(this->operators.begin(), this->operators.end(), nickname) == this->operators.end())
		throw std::invalid_argument("User is not operator");
	this->operators.erase(std::find(this->operators.begin(), this->operators.end(), nickname));
}

void			Channel::sendMessage(const Message &msg)
{
	for (size_t i = 0; i < this->users.size(); i++)
	{
		int socket = server->nick_to_sock[this->users[i]];
		server->users[socket].addOutMessage(msg);
	}
}
