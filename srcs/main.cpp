/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 15:35:17 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/02 10:01:20 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <unistd.h>

Server *server; // NOT SURE IF THIS IS ALLOWED

void signalHandler(int) {
    server->stop();
}

int main(int argc, char **argv) {
    signal(SIGINT, signalHandler);
    try
    {
        if (argc != 3)
            throw std::runtime_error("Usage: " + std::string(argv[0]) + " <port> <password>");
        Server server(SERVER_NAME , argv[1], argv[2]);
        ::server = &server;
        server.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        exit(EXIT_FAILURE);
    }
    return 0;
}
