/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 14:57:59 by tfriedri          #+#    #+#             */
/*   Updated: 2023/09/30 15:10:10 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define MAX_CLIENTS 10
#define SERVER_NAME "ft_irc"
#define BUFFER_SIZE 1024
#define END_OF_MESSAGE "\r\n" // some clients doesn't send "\r\n" but only "\n"