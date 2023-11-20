/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tilmanfs <tilmanfs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 12:39:09 by tfriedri          #+#    #+#             */
/*   Updated: 2023/11/20 23:55:51 by tilmanfs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

std::string     Server::name;
int             Server::port;
std::string     Server::password;

Server::Server()
{
    // Validate port
    if (this->port < 1024 || this->port > 65535)
        throw std::invalid_argument("Invalid port number");
    // Create socket and set options
    this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket < 0)
        throw std::runtime_error("Socket creation failed");
    int opt = 1;
    #ifdef __APPLE__
        if (setsockopt(this->socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
            throw std::runtime_error("Socket options setting failed");
    #else
        if (setsockopt(this->socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
            throw std::runtime_error("Socket options setting failed");
    #endif
    // Set server address
    memset(&this->address, 0, sizeof(this->address));
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(this->port);
    // Bind and listen
    if (bind(this->socket, (struct sockaddr *)&this->address, sizeof(this->address)) < 0)
        throw std::runtime_error("Socket binding failed");
    if (listen(this->socket, SOMAXCONN) < 0)
        throw std::runtime_error("Socket listening failed");
    // Add the server socket to poll fds
    pollfd pfd = {this->socket, POLLIN, 0};
    this->fds.push_back(pfd);
    // Flag server as running
    this->running = true;
    std::cout << "\033[1;32mServer " << this->name << " created on port " << this->port << "\033[0m" << std::endl;
}

Server::~Server()
{
}

void                    Server::setup(const char* name, const char* port, const char* password)
{
    Server::name = name;
    Server::port = atoi(port);
    Server::password = password;
}

Server&                 Server::getInstance()
{
    static Server instance;
    return instance;
}
    
std::string             Server::getPassword() const
{
    return (this->password);
}

void                    Server::run()
{
    signal(SIGPIPE, SIG_IGN);
    struct sockaddr_in      c_address;
    socklen_t               c_addrlen = sizeof(c_address);
    while (running)
    {
        if (poll(this->fds.data(), this->fds.size(), 0) < 0 && running == true)
            throw std::runtime_error("Polling failed");
        for (size_t i = 0; i < this->fds.size(); i++)
        {   
            if (this->fds[i].revents & POLLIN)
            {
                if (i == 0)
                    addNewUser(c_address, c_addrlen);
                else
                    receiveMessage(this->fds[i].fd);
            }
            else if (this->fds[i].revents & POLLOUT)
            {   
                std::string msg = this->users[this->fds[i].fd].getOutMessage().toString();
                while (msg != END_OF_MESSAGE && msg != "")
                {
                    ssize_t sent = send(this->fds[i].fd, msg.c_str(), msg.length() > OUT_BUFFER_SIZE ? OUT_BUFFER_SIZE : msg.length(), 0);
                    if (sent < 0)
                    {
                        std::cout << "\033[1;31mError sending message\033[0m" << std::endl;
                        removeUser(this->fds[i].fd);
                    }
                    else if (sent == 0)
                        std::cout << "\033[1;31mNothing sent\033[0m" << std::endl;
                    else
                    {
                        if (this->users[this->fds[i].fd].getRegistered() == true)
                            std::cout << "\033[0;33mTo\t" << this->users[this->fds[i].fd].getNickname() << ":\033[0m\t" << msg.substr(0, sent);
                        else
                            std::cout << "\033[0;33mTo\tsocket " << this->fds[i].fd << ":\033[0m\t" << msg.substr(0, sent);
                    }
                    msg.erase(0, sent); 
                }
            }
            else if (this->fds[i].revents & POLLERR)
            {
                std::cout << "Error on socket " << this->fds[i].fd << std::endl;
                if (this->fds[i].fd == this->socket)
                    stop();
                else
                    removeUser(this->fds[i].fd); 
            }
        }
    }
    disconnect();
}

void                    Server::stop()
{
    this->running = false;
}

void                    Server::disconnect()
{
    for (size_t i = 0; i < this->fds.size(); i++)
        close(this->fds[i].fd);
    std::cout << "\n\033[1;32mclosed all sockets\033[0m" << std::endl;
}

void                    Server::addNewUser(struct sockaddr_in address, socklen_t addrlen)
{
    try
    {
        int c_socket;
        if ((c_socket = accept(this->socket, (struct sockaddr *)&address, &addrlen)) < 0)
            throw std::runtime_error("Failed to accept connection");
        #ifdef __APPLE__
            fcntl(c_socket, F_SETFL, O_NONBLOCK);
        #endif
        pollfd pfd = {c_socket, POLLIN | POLLOUT, 0}; 
        this->fds.push_back(pfd);
        this->users.insert(std::pair<int, User>(c_socket, User(c_socket, inet_ntoa(address.sin_addr))));
		std::cout << "\033[1;32mSocket " << c_socket << ":\033[0m New connection from " << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void                    Server::registerUser(int socket)
{
    User &usr = this->users[socket];
    std::string nickname_upper = usr.getNickname();
    for (std::string::iterator it = nickname_upper.begin(); it != nickname_upper.end(); ++it) {
        *it = std::toupper(static_cast<unsigned char>(*it));
    }
    this->nick_to_sock.insert(std::pair<std::string, int>(nickname_upper, socket));
    usr.setRegistered(true);
	// send welcome messages
    usr.addOutMessage(Message::fromString(RPL_WELCOME(usr)));
	usr.addOutMessage(Message::fromString(RPL_YOURHOST(usr)));
	usr.addOutMessage(Message::fromString(RPL_ISSUPPORT(usr, "CHANTYPES=#& CHANMODES=,o,kl,it PREFIX=(o)@ NETWORK=" + this->name)));
	usr.addOutMessage(Message::fromString(RPL_MOTDSTART(usr)));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, ""))); 
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0@@@@@@\u00A0\u00A0\u00A0\u00A0\u00A0@@@@\u00A0\u00A0@@@@@@@")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0%@@@@@@\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0@@\u00A0\u00A0\u00A0\u00A0@@@@@@@")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0@@@@@@\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0@@@@@@")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0%@@@@@@\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0@@@@@@")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0\u00A0\u00A0\u00A0@@@@@@\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0@@@@@@")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0\u00A0\u00A0@@@@@@@@@@@@@@@@@@@\u00A0\u00A0\u00A0@@@@@@\u00A0\u00A0\u00A0\u00A0\u00A0@@")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0\u00A0\u00A0@@@@@@@@@@@@@@@@@@@\u00A0\u00A0\u00A0@@@@@@\u00A0\u00A0@@@@@")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0@@@@@@")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0@@@@@@")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "")));             
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0\u00A0\u00A0█████▒▄▄▄█████▓\u00A0\u00A0\u00A0\u00A0██▓\u00A0██▀███\u00A0\u00A0\u00A0▄████▄")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0▓██\u00A0\u00A0\u00A0▒\u00A0▓\u00A0\u00A0██▒\u00A0▓▒\u00A0\u00A0\u00A0▓██▒▓██\u00A0▒\u00A0██▒▒██▀\u00A0▀█")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0▒████\u00A0░\u00A0▒\u00A0▓██░\u00A0▒░\u00A0\u00A0\u00A0▒██▒▓██\u00A0░▄█\u00A0▒▒▓█\u00A0\u00A0\u00A0\u00A0▄")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0░▓█▒\u00A0\u00A0░\u00A0░\u00A0▓██▓\u00A0░\u00A0\u00A0\u00A0\u00A0░██░▒██▀▀█▄\u00A0\u00A0▒▓▓▄\u00A0▄██▒")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0░▒█░\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0▒██▒\u00A0░\u00A0\u00A0\u00A0\u00A0░██░░██▓\u00A0▒██▒▒\u00A0▓███▀\u00A0░")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0▒\u00A0░\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0▒\u00A0░░\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0░▓\u00A0\u00A0░\u00A0▒▓\u00A0░▒▓░░\u00A0░▒\u00A0▒\u00A0\u00A0░")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0░\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0░\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0▒\u00A0░\u00A0\u00A0░▒\u00A0░\u00A0▒░\u00A0\u00A0░\u00A0\u00A0▒")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0░\u00A0░\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0░\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0▒\u00A0░\u00A0\u00A0░░\u00A0\u00A0\u00A0░\u00A0░")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0░\u00A0\u00A0\u00A0\u00A0\u00A0░\u00A0\u00A0\u00A0\u00A0\u00A0░\u00A0░")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0\u00A0░")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0\u00A0\u00A0┏┓┏┓┏┓┳┓┏┓┳┓┏┳┓\u00A0\u00A0┏┓\u00A0\u00A0┏┳┓┏┓┳┓┳┏┓┳┓┳┓┳")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0\u00A0\u00A0┣┫┃┓┣\u00A0┣┫┣\u00A0┣┫\u00A0┃\u00A0\u00A0\u00A0┣╋\u00A0\u00A0\u00A0┃\u00A0┣\u00A0┣┫┃┣\u00A0┃┃┣┫┃")));
	usr.addOutMessage(Message::fromString(RPL_MOTD(usr, "\u00A0\u00A0\u00A0┛┗┗┛┗┛┻┛┗┛┛┗\u00A0┻\u00A0\u00A0\u00A0┗┻\u00A0\u00A0\u00A0┻\u00A0┻\u00A0┛┗┻┗┛┻┛┛┗┻")));
	usr.addOutMessage(Message::fromString(RPL_ENDOFMOTD(usr)));
	//
    std::cout << "\033[1;32mSocket " << socket << ":\033[0m Registered as " << usr.getNickname() << std::endl;
}

void                    Server::removeUser(int socket)
{
    // Print disconnect message | DEBUG
    if (this->users.find(socket) != this->users.end() && this->users[socket].getNickname() != "")
        std::cout << this->users[socket].getNickname() << " disconnected" << std::endl;
    else
        std::cout << "Socket " << socket << " disconnected" << std::endl;
    // remove user from fds vector
    for (size_t i = 1; i < this->fds.size(); i++)
    {
        if (this->fds[i].fd == socket)
        {
            this->fds.erase(fds.begin() + i);
            break;
        }
    }
    if (this->users[socket].getRegistered() == true)
    {
        // remove user from all channels
        for (size_t i = 0; i < this->users[socket].channels.size(); i++)
        {
            std::string channel_upper = this->users[socket].channels[i];
            for (std::string::iterator it = channel_upper.begin(); it != channel_upper.end(); ++it) {
                *it = std::toupper(static_cast<unsigned char>(*it));
            }
            this->channels[channel_upper].removeUser(this->users[socket], "");
            // check if channel is empty
            if (this->channels[channel_upper].users.size() == 0)
            {
                this->channels.erase(channel_upper);
                std::cout << "Channel " << channel_upper << " removed because it was empty" << std::endl;
                i--; // because the channel was removed, the next channel is now at the same index
            }
        }
        // remove user from nick_to_sock map
        for (std::map<std::string, int>::iterator it = this->nick_to_sock.begin(); it != this->nick_to_sock.end(); it++)
        {
            if (it->second == socket)
            {
                this->nick_to_sock.erase(it);
                break;
            }
        }
    }
    // remove user from users map
    this->users.erase(socket);
    // close user socket
    close(socket);
}

void                    Server::receiveMessage(int socket)
{
    char buffer[BUFFER_SIZE] = {0};
    ssize_t valread = recv(socket, buffer, BUFFER_SIZE - 1, 0);
    if (valread < 0) // error
        removeUser(socket);
    else if (valread == 0) // client disconnected
        removeUser(socket);
    else
    {
        std::cout << buffer << std::endl;
        User &usr = this->users[socket];
        usr.in_buffer.append(std::string(buffer));
        while (this->users.find(socket) != this->users.end() && usr.in_buffer.find(END_OF_MESSAGE) != std::string::npos) // while there are full messages in buffer
        {
            std::string msg_str = usr.in_buffer.substr(0, usr.in_buffer.find(END_OF_MESSAGE));
            usr.in_buffer.erase(0, usr.in_buffer.find(END_OF_MESSAGE) + strlen(END_OF_MESSAGE));
            Message msg = Message::fromString(msg_str);
            if (usr.getRegistered() == true)
                std::cout << "\033[0;33mFrom\t" << usr.getNickname() << ":\033[0m\t" << msg_str << std::endl;
            else
                std::cout << "\033[0;33mFrom\tsocket" << socket << ":\033[0m\t" << msg_str << std::endl;
            handleMessage(msg, usr);
        }
    }
}

void                    Server::handleMessage(Message &msg, User &usr)
{
    // Important:
    // -The client must send a PASS command before sending the NICK and USER commands.
    // -The client must send a NICK and USER command before any others.
    std::string cmmnd = msg.getCommand();
    if (cmmnd == "CAP") // ignore CAP messages
		return ;
    else if (cmmnd == "QUIT")
        quit(usr);
	else if (cmmnd == "PING")
		ping(msg, usr);
	else if (usr.getVerified() == false) // only allow PASS command
	{
		if (cmmnd == "PASS")
			pass(msg, usr);
		else
			removeUser(usr.getSocket());
	}
	else
	{
		if (cmmnd == "NICK")
			nick(msg, usr);
		else if (cmmnd == "USER")
			user(msg, usr);
		else if (usr.getRegistered() == false)
			return ;
		else if (msg.getCommand() == "JOIN")
			join(msg, usr);
		else if (msg.getCommand() == "PRIVMSG")
			privmsg(msg, usr);
		else if (msg.getCommand() == "NOTICE")
			notice(msg, usr);
		else if (msg.getCommand() == "PART")
			part(msg, usr);
		else if (msg.getCommand() == "TOPIC")
			topic(msg, usr);
		else if (msg.getCommand() == "KICK")
			kick(msg, usr);
		else if (msg.getCommand() == "NAMES")
			names(msg, usr);
		else if (msg.getCommand() == "MODE")
			mode(msg, usr);
        else if (msg.getCommand() == "INVITE")
            invite(msg, usr);
        else if (msg.getCommand() == "LIST")
            list(msg, usr);
		else
			usr.addOutMessage(Message::fromString(ERR_UNKNOWNCOMMAND(usr, msg.getCommand())));
	}
}
