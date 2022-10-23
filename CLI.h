/*
 * CLI.h
 *
 * Author:308540343 , Maor Biton.
 */
#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include <vector>
#include "commands.h"
#include "timeseries.h"

using namespace std;

class CLI {
    DefaultIO *dio;
    DefaultIO *d;
    vector<Command *> commands;
    initialization *init;
    vector<pair<int, Command *>> indexBycomm;

public:
    CLI(DefaultIO *dio);

    void start();

    void insertCommand(Command *command);

    void genCommand();

    void printCommand();

    virtual ~CLI();

    void getCommend(int index, string comm);
};

#endif /* CLI_H_ */