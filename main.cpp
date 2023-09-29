/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 15:35:17 by tfriedri          #+#    #+#             */
/*   Updated: 2023/09/29 20:28:46 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <signal.h>

int main(int argc, char **argv) {
    try
    {
        if (argc != 3)
            throw std::runtime_error("Usage: " + std::string(argv[0]) + " <port> <password>");
        Server server(SERVER_NAME , argv[1], argv[2]);
        server.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        exit(EXIT_FAILURE);
    }

    return 0;
}
