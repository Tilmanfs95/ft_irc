/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 00:12:48 by tfriedri          #+#    #+#             */
/*   Updated: 2023/10/03 00:01:18 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include <vector>

class Channel
{
private:
    // channel name:
    // -max 50 chars
    // -must start with # or &
    // -must not contain ' ' , ',' , ':' , '^G' or ASCII 7
    // -must be unique
    // -not case sensitive
    std::string name;
    // channel topic:
    std::string topic;
    // channel password:
    // -case sensitive
    std::string password;
    // user list:
    std::vector<std::string> users;
    // operator list:
    std::vector<std::string> operators;
    // invite list:
    std::vector<std::string> invites;
    // user limit:
    unsigned int limit;
    // topic restrict:
    // -if true, only operators can change the topic
    bool topic_restrict;
    // invite only:
    // -if true, only invited users can join
    bool invite_only;
    
public:
    Channel(/* args */);
    ~Channel();
};

#endif
