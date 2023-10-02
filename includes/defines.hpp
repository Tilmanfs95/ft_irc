/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 14:57:59 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/02 10:31:17 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#define SERVER_NAME "ft_irc"
#define BUFFER_SIZE 1024
#define OUT_BUFFER_SIZE 512
#define END_OF_MESSAGE "\n" // some clients doesn't send "\r\n" but only "\n"