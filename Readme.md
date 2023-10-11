# FT_IRC

## Links:

    - https://www.rfc-editor.org/rfc/rfc2812.html
    - https://cdn.intra.42.fr/pdf/pdf/97375/en.subject.pdf

## TODO:

    - [ ] check if it is running on MacOS (I think we need to change something that it is NON-BLOCKING --> look at the subject!)
    - [ ] test the partial data receiving
    - [ ] check if the signal handler is complete
    - [ ] check if poll() is used correctly (especially the timeout !)
    - [ ] Answer PING messages from clients
    - [ ] if possible, reverse DNS lookup for hostname of clients (currently the ip is used as alternative)
    - [ ] in Server::receiveMessage() loop until all data is received before processing it
    - [ ] check all used libraries for c++ alternatives (e.g. <unistd.h> in main.cpp...)
    - [ ] send all the info stuff to the client after successful registration
    - [ ] send the needed info to the client after joining a channel
    - [ ] need to anwser the "MODE #channel" that comes from client after joining a channel ??
    - [ ] implement PRIVMSG
    - [ ] implement NOTICE (maybe?)
    - [ ] implement PART
    - [ ] implement TOPIC
    - [ ] implement NAMES
    - [ ] implement LIST
    - [ ] implement INVITE
    - [ ] implement ALL THE STUFF FROM THE SUBJECT

    - [ ] maybe make it possible to start without a password? (not sure if conform with the subject)
    - [ ] Bonus ?
