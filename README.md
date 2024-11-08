# IRC Server Project

![IRC Logo](img/irclogo.webp)

## Introduction

This is a group-project for [42Perpignan](https://42perpignan.fr/) school's curriculum.

Created by [mrabat](https://github.com/rabatm) and [svanmarc](https://github.com/svanmarc)

It is about implementing a IRC-server, designed to handle multiple clients without hanging and uses non-blocking I/O
operations,
following the [Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459#section-1.1).

### Requirements

- C++ 98 compiler
- An IRC client ([IRSS](https://irssi.org/) recommended)

### Usage

To compile and run the server, execute the following commands:

```
make
./ircserv <port> <password>
```

- `port`: The port number on which your IRC server will be listening for incoming IRC connections.
- `password`: The connection password required by any IRC client trying to connect to your server.

### Features

- Authenticate, set a nickname, and a username
- Join a channel
- Send and receive private messages
- Forward messages from one client to all other clients in the channel
- Support for operators and regular users

### Commands

the right syntax for the listed commands is explained in
the [protocol](https://datatracker.ietf.org/doc/html/rfc1459#section-4.4.2).

#### Registration Commands

- `PASS`: Requires password
- `NICK`: Set nickname
- `USER`: Set Username

#### Basic Commands

- `NICK`: Set nickname
- `PRIVMSG`: Sends a message to recipient
- `NOTICE`: Sends a message to recipient (slightly different behavior)
- `LIST`: Lists all the channels
- `WHO` : Lists all the users in a channel
- `TOPIC`: Show the topic of a channel
- `JOIN`: Join a channel
- `PART`: Leave a channel
- `QUIT`: Leave the server

#### Channel Operator Commands

- `KICK`: Eject a client from the channel
- `INVITE`: Invite a client to a channel
- `TOPIC`: Change or view the channel topic
- `MODE`: Change the channel's mode
    - `i`: Set/remove Invite-only channel
    - `t`: Set/remove the restrictions of the TOPIC command to channel operators
    - `k`: Set/remove the channel key (password)
    - `o`: Give/take channel operator privilege
    - `l`: Set/remove the user limit to the channel

#### Demonstration

![IRC Demo](img/ircdemo.gif)