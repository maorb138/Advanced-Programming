/*
 * CLI.cpp
 *
 * Author:308540343 , Maor Biton.
 */
#include "CLI.h"
#include "vector"


CLI::CLI(DefaultIO *dio) : dio(dio) {
    init = new initialization(new HybridAnomalyDetector);
    genCommand();
}

void CLI::start() {
    float optTemp;
    int opt;
    int size = commands.size();

    while (true) {
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        printCommand();
        dio->read(&optTemp);
        opt = (int) optTemp - 1;
        if (opt == 5) break;
        else if (opt < 0 || opt >= size) {
            dio->write("invalid option. Please choose a valid option.\n");
        } else
            commands[opt]->execute();
    }
}

void CLI::insertCommand(Command *command) {
    commands.push_back(command);
}

void CLI::genCommand() {
    insertCommand(new UploadCsvFile(dio, init));
    insertCommand(new setAlgo(dio, init));
    insertCommand(new DetectAnomalies(dio, init));
    insertCommand(new DisplayResults(dio, init));
    insertCommand(new CalculateResults(dio, init));
    insertCommand(new Exit(dio, init));
}

void CLI::printCommand() {
    int size = commands.size();
    for (int i = 0; i < size; i++) {
        dio->write(i + 1);
        string opt = "." + commands[i]->getDescription() + "\n";
        dio->write(opt);

    }
}



CLI::~CLI() {
    int size = commands.size();
    for (int i = 0; i < size; i++) {
        delete commands[i];
    }
    delete init;
}
