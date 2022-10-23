/*
 * Server.cpp
 *
 * Author:308540343 , Maor Biton.
 */
#include "Server.h"

Server::Server(int port) throw(const char *) {
    run = true;
    !handShack;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
        throw runtime_error("Failed to create socket");
    serSock.sin_family = AF_INET;
    serSock.sin_addr.s_addr = INADDR_ANY;
    serSock.sin_port = htons(port);
    if (bind(sock_fd, (struct sockaddr *) &serSock, sizeof(serSock)) < 0)
        throw runtime_error("Failed to bind server.");

    if (listen(sock_fd, 3) < 0)
        throw runtime_error("Failed to listen.");

}

void Server::start(ClientHandler &ch) throw(const char *) {
    t = new thread([&ch, this]() {
        socklen_t size = sizeof(cliSock);
        while (run) {
            alarm(1);
            int client = accept(sock_fd, (struct sockaddr *) &cliSock, &size);
            alarm(0);
            if (client < 0)
                throw runtime_error("Failed to accept client.");
            ch.handle(client);
            close(client);

        }
    });
}

void Server::stop() {
    run = false;
    t->join(); // do not delete this!
    close(sock_fd);
}

Server::~Server() {
    delete t;
}