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
    - [ ] check if the signal handler is complete (only SIGINT is handled)
    - [ ] POLL timeout 0 , 1 oder -1 ??????
    - [ ] check all used libraries for c++ alternatives (e.g. <unistd.h> in main.cpp...)
    - [ ] implement ALL THE STUFF FROM THE SUBJECT
    - [ ] Check if bot is running good!
	- [ ] forbid to send messages to channels if not in it ? (most irc-servers allow it?!)
