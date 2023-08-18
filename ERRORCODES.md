# Error codes for the API

> Differing from the protocol described by @freehuntx, I send the shorttext to the error code as a second argument.

## list of error codes and descriptions

EC | Description
--- | ---
ARG_ERR | Argument error, see sent shorttext for details
UNKNOWN_TYPE | Ypu sent an unknown packet type. Each packet starts with the packet type followed by a `\|`. See the protocol for more details.