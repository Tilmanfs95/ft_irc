<img src="https://cdn.simpleicons.org/42/000/fff" alt="42" width=100>

# FT_IRC
Internet Relay Chat

Summary:
This project is about creating your own IRC server.
You will use an actual IRC client to connect to your server and test it.
Internet is ruled by solid standards protocols that allow connected computers to interact
with each other.
It’s always a good thing to know.

Version: 8

![Static Badge](https://img.shields.io/badge/SUCCESS-125-darkgreen?style=for-the-badge&logo=42)

## helpful links:

- https://modern.ircdocs.horse/#nick-message
- https://www.rfc-editor.org/rfc/rfc2812.html
- https://cdn.intra.42.fr/pdf/pdf/97375/en.subject.pdf

## helpful tips:
- Start WeeChat multiple times on MacOS: (for testing multiple clients)
    - In Terminal: `weechat --dir ~/weechat2` (creates a new config directory in ~/weechat2)
    
- Disable flood protection in WeeChat:
    -   /set irc.server_default.anti_flood_prio_high 0
    -   /set irc.server_default.anti_flood_prio_low 0

- Send Files with WeeChat:
    -   /dcc send <nick> <file>
    
- Accept/Receive Files with WeeChat:
    -   navigate to the xfer.list buffer and press A + Enter at the file you want to accept
    
- For "direct" testing without irc-client use netcat with the c-flag (nc -c) to use carriage return and newline ("\r\n") as line endings (or change END_OF_MESSAGE in defines.hpp to "\n")
