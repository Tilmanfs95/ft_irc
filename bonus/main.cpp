/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tilmanfs <tilmanfs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 19:19:16 by tilmanfs          #+#    #+#             */
/*   Updated: 2023/11/21 20:38:02 by tilmanfs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

void signalHandler(int) {
    Bot::getInstance().stop();
}

int main(int argc, char **argv)
{
    signal(SIGINT, signalHandler);
    try
    {
        if (argc != 4)
            throw std::runtime_error("Usage: " + std::string(argv[0]) + " <server> <port> <password>");
        Bot::setup(argv[1], argv[2], argv[3]);
        Bot::getInstance().run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        exit(EXIT_FAILURE);
    }
    return 0;
}