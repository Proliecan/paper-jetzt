---
    creator: freehuntx
    creator-git: https://github.com/freehuntx
    source: https://raw.githubusercontent.com/freehuntx/gpn-tron/master/PROTOCOL.md
    date-of-access: 2023-08-18
    license: MIT
    license-url: https://raw.githubusercontent.com/freehuntx/gpn-tron/master/LICENSE
    copyright-notice: Copyright (c) 2023 freehuntx
    license-text: |
        MIT License

        Permission is hereby granted, free of charge, to any person obtaining a copy
        of this software and associated documentation files (the "Software"), to deal
        in the Software without restriction, including without limitation the rights
        to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
        copies of the Software, and to permit persons to whom the Software is
        furnished to do so, subject to the following conditions:
        
        The above copyright notice and this notice shall be
        included in all copies or substantial portions of the Software.
        
        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
        IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
        FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
        AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
        LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
        OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
        SOFTWARE. 
---

# Protocol


The protocol is string based.  
Every packet must and will end with a newline (\n).  
E.g: `pos|10|50|5\n`  
**Note:** _All next examples in this documentation are given without \n. But dont forget it in your parsing!_

## Packet structure

The general packet structure looks like this:  
\<packet type\>\<...arguments\>  
E.g: `game|5|1|2|3`  
Where game is the packet type, 5 the first argument, 1 the second, 2 the third and 3 the fourth

## Packet types

### motd

The motd packet is sent by the server when you connect to it.  
motd means "Message of the day".

**Name:** motd  
**Sender:** Server  
**Arguments:**  
| # | Type | Description |
|---|--------|------------------------|
| 1 | String | The message of the day |

**Example:** `motd|Hello how are you? :)`

### join

The join packet is the first packet the client has to send to the server when connecting.  
Remember the password otherwise you cant use the username again!

**Name:** join  
**Sender:** Client  
**Arguments:**  
| # | Type | Description |
|---|--------|--------------|
| 1 | String | The username |
| 2 | String | The password |

**Example:** `join|Cool Guy|mysupersecret`

### error

The error packet is sent by the server if something went wrong.

**Name:** error  
**Sender:** Server  
**Arguments:**  
| # | Type | Description |
|---|--------|--------------|
| 1 | String | The error according to [ERRORCODES.md](ERRORCODES.md) |

**Example:** `error|INVALID_USERNAME`

### game

The game packet is sent by the server to inform the client about the new game round.  
It contains information about the map size and the current player id.

**Name:** game  
**Sender:** Server  
**Arguments:**  
| # | Type | Description |
|---|--------|-------------------------------|
| 1 | Number | The width of the current map |
| 2 | Number | The height of the current map |
| 3 | Number | The current player id |

**Example:** `game|100|100|5`

### pos

The pos packet is sent by the server to inform the client about a players current position.

**Name:** pos  
**Sender:** Server  
**Arguments:**  
| # | Type | Description |
|---|--------|--------------------------------------------------------------------|
| 1 | Number | The player id |
| 2 | Number | x position of the player |
| 3 | Number | y position of the player |

**Example:** `pos|5|3|8`

### player

The player packet is sent by the server to share informations of an player.

**Name:** player  
**Sender:** Server  
**Arguments:**  
| # | Type | Description |
|---|--------|--------------------------------------------------------------------|
| 1 | Number | The player id |
| 2 | String | The name of the player |

**Example:** `player|3|Coolguy`

### tick

The tick packet is sent by the server after a turn has been done. Its the best to send a move packet after this!

**Name:** tick  
**Sender:** Server  
**Arguments:**  None

**Example:** `tick`

### die

The die packet is sent by the server to inform the client about a players who died.

**Name:** die  
**Sender:** Server  
**Arguments:**  
| # | Type | Description |
|---|--------|--------------------------------------------------------------------|
| 1... | Number | The player id |

**Example (1 dead player):** `die|5`  
**Example (4 dead player):** `die|5|8|9|13`

### move

The move packet is sent by the client to decide where to move.

**Name:** move  
**Sender:** Client  
**Arguments:**  
| # | Type | Description |
|---|--------|-------------------------|
| 1 | String | up, right, down or left |

**Example:** `move|up`

### chat

The chat packet is sent by the client to send a cool chat message :>.

**Name:** chat  
**Sender:** Client  
**Arguments:**  
| # | Type | Description |
|---|--------|-----------------------------|
| 1 | String | The chat message to display |

**Example:** `chat|I am so cool`

### message

The message packet is sent by the server to inform about a chat message of another player

**Name:** message  
**Sender:** Server  
**Arguments:**  
| # | Type | Description |
|---|--------|-----------------------------|
| 1 | Number | The player id |
| 2 | String | The chat message to display |

**Example:** `message|7|I am so cool`

### win

The win packet is sent by the server to inform the client they won.

**Name:** win  
**Sender:** Server  
**Arguments:**  
| # | Type | Description |
|---|--------|-----------------|
| 1 | Number | amount of wins |
| 2 | Number | amount of losses |

**Example:** `win|1|20`

### lose

The lose packet is sent by the server to inform the client they lost.

**Name:** lose  
**Sender:** Server  
**Arguments:**  
| # | Type | Description |
|---|--------|-----------------|
| 1 | Number | amount of wins |
| 2 | Number | amount of losses |

**Example:** `lose|1|20`