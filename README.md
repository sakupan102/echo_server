# TL;DR
An echo server implementation in C. Messages sent by the client are returned as-is in the response.
# Implementation Status
- Done
    - Error handling
    - Sending and receiving messages
    - DNS resolution
- ToDo
    - IPv6 support
    - Handling a large volume of requests
    - Graceful Shutdown

# Usage
## Starting the echo server
```
make run-server
```
## Sending a request from the client
```
make run-client MSG="message to send" DOMAIN=domain_name
```
