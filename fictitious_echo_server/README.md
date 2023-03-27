## The Echo Protocol

In C, write a server and client that implement the fictitious "echo protocol". To implement the protocol, the server should accept any string from the client, and then return that string with all letters capitalized (if letters exist).

### Echo Protocol Example

- Client sends "Hello, wOrlD"
- Server echoes "HELLO, WORLD"

As soon as the server responds to a client, it may close. And, as soon as the clients receives a response, it may close.
