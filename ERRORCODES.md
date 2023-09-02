# Error codes for the API

> Differing from the protocol described by @freehuntx, I send the shorttext to the error code as a second argument.

## list of error codes and descriptions

EC | Description
--- | ---
ARG_ERR | Argument error, see sent shorttext for details
UNKNOWN_TYPE | Ypu sent an unknown packet type. Each packet starts with the packet type followed by a `\|`. See the protocol for more details.
ALREADY_JOINED | You already joined the game
NOT_JOINED | You have not joined the game yet
AUTH_ERR | Authentication error; invalid username or password. Please be aware that the password is case-sensitive, cannot contain any spaces and cannot be changed.
NOT_IMPLEMENTED | The requested feature is not implemented (yet). *Implement it and open a pull request to receive a honorable mention in the readme and MOTD.*