/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 12:39:09 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/11 20:06:40 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"


Server::Server(/* args */)
{
}

Server::Server(const char* name, const char* port, const char* password)
    : name(name), port(atoi(port)), password(password)
{
    if (this->port < 1024 || this->port > 65535)
        throw std::invalid_argument("Invalid port number");
    // Create socket
    if ((this->socket = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("Socket creation failed");
    // set socket options
    int opt = 1;
    if (setsockopt(this->socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
        throw std::runtime_error("Socket options setting failed");
    // set server address
    memset(&this->address, 0, sizeof(this->address));
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(this->port);
    if (this->address.sin_port == 0)
        throw std::runtime_error("Invalid port number");
    // bind socket to address
    if (bind(this->socket, (struct sockaddr *)&this->address, sizeof(this->address)) < 0)
        throw std::runtime_error("Socket binding failed");
    // listen for connections
    if (listen(this->socket, SOMAXCONN) < 0)
        throw std::runtime_error("Socket listening failed");
    pollfd pfd = {this->socket, POLLIN, 0};
    this->fds.push_back(pfd);
    // set running to true
    this->running = true;
    // std::cout << "Server created" << std::endl;
    std::cout << "\033[1;32mServer " << this->name << " created on port " << this->port << "\033[0m" << std::endl;
}

Server::~Server()
{
}

std::string             Server::getPassword() const
{
    return (this->password);
}

void                    Server::run()
{
    struct sockaddr_in      c_address;
    socklen_t               c_addrlen = sizeof(c_address);
    while (running)
    {
        if (poll(this->fds.data(), this->fds.size(), 0) < 0 && running == true) // ore use another timeout?
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
                try
                {
                    std::string msg = this->users[this->fds[i].fd].getOutMessage().toString();
                    while (msg != "")
                    {
						ssize_t sent = send(this->fds[i].fd, msg.c_str(), msg.length() > OUT_BUFFER_SIZE ? OUT_BUFFER_SIZE : msg.length(), 0);
                        if (sent < 0)
							std::cout << "\033[1;31mError sending message\033[0m" << std::endl;
						else if (sent == 0)
							std::cout << "\033[1;31mNothing sent\033[0m" << std::endl;
                        else
						{
                            if (this->users[this->fds[i].fd].getRegistered() == true)
                                std::cout << "\033[0;33mTo\t" << this->users[this->fds[i].fd].getNickname() << ":\033[0m\t" << msg.substr(0, sent);
                            else
                                std::cout << "\033[0;33mTo\tsocket " << this->fds[i].fd << ":\033[0m\t" << msg.substr(0, sent);
							msg.erase(0, sent);
						}
                    }
                }
                catch(const std::exception& e)
                {
                    // std::cerr << e.what() << '\n';
                }
            }
            // else if (this->fds[i].revents & POLLERR)
            // {
            //     std::cout << "Error on socket " << this->fds[i].fd << std::endl;
            //     // what to do here?
            // }
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
    std::cout << "\n\033[1;32mServer stopped successfully\033[0m\n" << std::endl;
}

void                    Server::addNewUser(struct sockaddr_in address, socklen_t addrlen)
{
    try
    {
        int c_socket;
        if ((c_socket = accept(this->socket, (struct sockaddr *)&address, &addrlen)) < 0)
            throw std::runtime_error("Failed to accept connection");
        pollfd pfd = {c_socket, POLLIN | POLLOUT, 0}; 
        this->fds.push_back(pfd);
        // std::string ip = inet_ntoa(address.sin_addr);
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
    std::transform(nickname_upper.begin(), nickname_upper.end(), nickname_upper.begin(), ::toupper);
    this->nick_to_sock.insert(std::pair<std::string, int>(nickname_upper, socket));
    usr.setRegistered(true);
    usr.addOutMessage(Message::fromString(RPL_WELCOME(usr)));
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
            std::transform(channel_upper.begin(), channel_upper.end(), channel_upper.begin(), ::toupper);
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
    if (valread < 0)
    {
        // how to handle this situation if we could not read from socket?
        std::cerr << "Error reading from socket" << std::endl;
    }
    else if (valread == 0) // client disconnected
        removeUser(socket);
    else
    {
        User &usr = this->users[socket];
        usr.in_buffer.append(std::string(buffer));
        while (this->users.find(socket) != this->users.end() && usr.in_buffer.find(END_OF_MESSAGE) != std::string::npos) // while there are full messages in buffer
        {
            std::string msg_str = usr.in_buffer.substr(0, usr.in_buffer.find(END_OF_MESSAGE));
            usr.in_buffer.erase(0, usr.in_buffer.find(END_OF_MESSAGE) + strlen(END_OF_MESSAGE));
            Message msg = Message::fromString(msg_str);
            // msg.print();
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
        quit(msg, usr);
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
		// ...
		// ..
		// .
		else
			usr.addOutMessage(Message::fromString(ERR_UNKNOWNCOMMAND(usr, msg.getCommand())));
	}
}
