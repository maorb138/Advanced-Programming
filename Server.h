/*
 * Server.h
 *
 * Author:308540343 , Maor Biton.
 */
#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "CLI.h"
#include "commands.h"

using namespace std;

// edit your ClientHandler interface here:
class ClientHandler {
public:
    virtual void handle(int clientID) = 0;
};

class AnomalyDetectionHandler : public ClientHandler {
public:
    virtual void handle(int clientID) {
        CLI *cli = new CLI(new SocketDio(clientID));
        cli->start();
        delete cli;
    }
};


// implement on Server.cpp
class Server {
    thread *t;
    int sock_fd;
    string path;
    sockaddr_in serSock;
    sockaddr_in cliSock;
    bool run;
    bool handShack = true;

public:
    Server(int port) throw(const char *);

    virtual ~Server();

    void start(ClientHandler &ch) throw(const char *);

    void stop();
};

#endif /* SERVER_H_ */