# FT_IRC

## Links:

	- https://modern.ircdocs.horse/#nick-message
    - https://www.rfc-editor.org/rfc/rfc2812.html
    - https://cdn.intra.42.fr/pdf/pdf/97375/en.subject.pdf

## Tricks:
    - Start WeeChat multiple times on MacOS: (for testing multiple clients)
        - In Terminal: `weechat --dir ~/weechat2` (creates a new config directory in ~/weechat2)
    
    - Disable flood protection in WeeChat:
        -   /set irc.server_default.anti_flood_prio_high 0
        -   /set irc.server_default.anti_flood_prio_low 0

    - Send Files with WeeChat:
        -   /dcc send <nick> <file>
    
    - Accept/Receive Files with WeeChat:
        -   navigate to the xfer.list buffer and press A + Enter at the file you want to accept
    

## TODO:

    - [ ] test the partial data receiving
    - [ ] check if the signal handler is complete
    - [ ] check if poll() is used correctly (especially the timeout !)
    - [ ] in Server::receiveMessage() loop until all data is received before processing it
    - [ ] check all used libraries for c++ alternatives (e.g. <unistd.h> in main.cpp...)
    - [ ] send all the info stuff to the client after successful registration
    - [ ] send the needed info to the client after joining a channel
    - [ ] need to anwser the "MODE #channel" that comes from client after joining a channel ??
    - [ ] implement NAMES
    - [ ] implement LIST
    - [ ] implement INVITE
    - [ ] implement ALL THE STUFF FROM THE SUBJECT
    - [ ] maybe make it possible to start without a password? (not sure if conform with the subject)
    - [ ] Bonus ?
