/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 15:35:17 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/29 16:54:00 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <unistd.h>

void signalHandler(int) {
    Server::getInstance().stop();
}

int main(int argc, char **argv) {
    signal(SIGINT, signalHandler);
    try
    {
        if (argc != 3)
            throw std::runtime_error("Usage: " + std::string(argv[0]) + " <port> <password>");
        Server::setup(SERVER_NAME, argv[1], argv[2]);
        Server::getInstance().run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        exit(EXIT_FAILURE);
    }
    return 0;
}
