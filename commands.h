/*
 * commands.h
 *
 * Author:308540343 , Maor Biton.
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "exception"
#include <fstream>
#include <utility>
#include <vector>
#include <sstream>
#include "HybridAnomalyDetector.h"


using namespace std;

class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void read(string *text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    virtual void write(int f) = 0;

    virtual ~DefaultIO() = default;
};

class SocketDio : public DefaultIO {
    int clientID;
public:
    SocketDio(int clientID) : clientID(clientID) {}

    string recNewline() const {
        string msg = "";
        int i = 0;
        char buffer[1];
        while (true) {
            if (recv(clientID, buffer, 1, 0) < 0)
                throw runtime_error("Failed to receive from client.");
            if (buffer[0] == '\n')
                break;
            msg += buffer[0];
            i++;
        }

        return msg;
    }

    string read() override {
        return recNewline();
    }

    void read(float *f) override {
        string msg = recNewline();
        *f = stof(msg);
    }

    void read(string *text) override {
        string line = recNewline();
        *text = stof(line);

    }

    void write(string text) override {
        char *msg = &text[0];
        int msg_len = strlen(msg);
        if (send(clientID, msg, msg_len, 0) < 0)
            throw runtime_error("Failed to write to client.");
    }

    void write(float f) override {
        stringstream ss;
        ss << f;
        string msg_t(ss.str());
        char *msg = &msg_t[0];
        int msg_len = strlen(msg);
        if (send(clientID, msg, msg_len, 0) < 0)
            throw runtime_error("Failed to write to client.");
    }

    void write(int f) override {
        stringstream ss;
        ss << f;
        string msg_t(ss.str());
        char *msg = &msg_t[0];
        int msg_len = strlen(msg);
        if (send(clientID, msg, msg_len, 0) < 0)
            throw runtime_error("Failed to write to client.");
    }
};

class initialization {
    SimpleAnomalyDetector *detector;
    int timeSteps;
    vector<AnomalyReport> anomalies;

public:
    explicit initialization(SimpleAnomalyDetector *detector) : detector(detector), timeSteps(0),
                                                               anomalies(vector<AnomalyReport>{}) {}

    float get_threshold() { return detector->getThrehold(); }

    void set_threshold(float t) { detector->setThrehold(t); }

    vector<AnomalyReport> getAnomalies() { return anomalies; }

    int getTimeSteps() const { return timeSteps; }

    int getTimeTrain() const { return getTimeTrain(); }


    static vector<string> readFromV(map<int, string> att) {
        string line, param;
        vector<string> comm;
        int size = att.size();
        vector<int> indexCom(5);
        for (int i = 0; i < 5; i++) {
            indexCom[i] = i;
        }
        for (int i = 0; i < size; i++) {
            comm.push_back(att[i]);
        }
        return comm;

    }

    static vector<pair<string, vector<float>>> read_data(DefaultIO *dio) {
        vector<pair<string, vector<float>>> data;
        string line, parameterName;
        map<int, string> indexByStr;
        insertS(indexByStr);
        readFromV(indexByStr);
        line = dio->read();
        for (int i = 0; i < indexByStr.size(); i++) {
            getCom(i, ",", indexByStr);
        }
        stringstream stream(line);
        while (getline(stream, parameterName, ',')) {
            data.push_back(pair<string, vector<float>>(parameterName, vector<float>{}));
        }

        float val;
        while (true) {
            line = dio->read();
            if (line.compare("done") == 0)
                break;
            stringstream stream(line);
            int collumIndex = 0;
            while (stream >> val) {
                data.at(collumIndex).second.push_back(val);
                if (stream.peek() == ',')
                    stream.ignore();
                collumIndex++;
            }
        }
        return data;
    }


    static void writeToCsv(string file_name, vector<pair<string, vector<float>>> data) {
        ofstream file(file_name);
        if (!file.is_open())
            throw runtime_error("enable open file\n");
        int col_amount = data.size();
        for (int k = 0; k < col_amount; k++) {
            file << data.at(k).first;
            if (k != (col_amount - 1))
                file << ",";
        }
        file << "\n";

        for (int i = 0; i < data.at(0).second.size(); ++i) {
            for (int j = 0; j < col_amount; ++j) {
                file << data.at(j).second.at(i);
                if (j != (col_amount - 1))
                    file << ",";
            }
            file << "\n";
        }
        file.close();
    }

    static void writeToCsv(string file_name, DefaultIO *dio) {
        writeToCsv(file_name, read_data(dio));
    }

    void analyze(const char *train_file_name, const char *test_file_name) {
        const TimeSeries train_ts(train_file_name);
        const TimeSeries test_ts(test_file_name);
        timeSteps = test_ts.listSize();
        int timetrain = train_ts.listSize();
        int coltest = test_ts.numOfCol();
        int coltrain = train_ts.numOfCol();
        detector->learnNormal(train_ts);
        anomalies = detector->detect(test_ts);
    }

    static string getCom(int index, string com, map<int, string> att) {
        return att.at(index);
    }

    static map<key_t, string> insertS(map<key_t, string> indexByStr) {
        indexByStr[1] = "UploadCsvFile";
        indexByStr[2] = "setAlgo";
        indexByStr[3] = "DetectAnomalies";
        indexByStr[4] = "DisplayResults";
        indexByStr[5] = "CalculateResults";
        indexByStr[6] = "Exit";
        return indexByStr;

    }


    ~initialization() { delete detector; }
};

class Command {
protected:
    DefaultIO *dio;
    DefaultIO *d;
    initialization *init;
    string description;
    float thr = 0.9;
public:
    Command(DefaultIO *dio, initialization *init, string description) : dio(dio), init(init),
                                                                        description(std::move(description)) {}

    virtual void execute() = 0;

    virtual ~Command() = default;

    string getDescription() { return description; }

};


class UploadCsvFile : public Command {
private:
    TimeSeries *testFile;
    TimeSeries *trainFile;
public:
    UploadCsvFile(DefaultIO *dio, initialization *init) : Command(dio, init, "upload a time series csv file") {}

    void execute() override {
        dio->write("Please upload your local train CSV file.\n");
        initialization::writeToCsv("anomalyTrain.csv", dio);
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        initialization::writeToCsv("anomalyTest.csv", dio);
        dio->write("Upload complete.\n");
    }

    ~UploadCsvFile() {
        delete testFile;
        delete trainFile;
    }

};

class Exit : public Command {
public:
    Exit(DefaultIO *dio, initialization *init) : Command(dio, init, "exit") {}

    void execute() override {}


};


class setAlgo : public Command {
public:
    setAlgo(DefaultIO *dio, initialization *init) : Command(dio, init, "algorithm settings") {}

    void execute() override {
        float th;
        bool legal = false;
        while (!legal) {
            dio->write("The current correlation threshold is ");
            dio->write(init->get_threshold());
            dio->write("\nType a new threshold\n");
            dio->read(&th);
            if (th >= 0 && th <= 1) {
                init->set_threshold(th);
                legal = true;
            }
            if (th < 0 && th > 1) { // bad threshold
                dio->write("please choose a value between 0 and 1.\n");
            }
        }
    }
};

class CalculateResults : public Command {
public:
    CalculateResults(DefaultIO *dio, initialization *init) :
            Command(dio, init, "upload anomalies and analyze results") {}

    vector<pair<int, int>> unifyAnom() {
        vector<AnomalyReport> anomalies = init->getAnomalies();
        vector<pair<int, int>> unified;
        int size = anomalies.size();
        int start = 0;
        for (int i = 1; i < size; i++) {
            if (anomalies[i].timeStep == anomalies[i - 1].timeStep + 1
                && anomalies[i].description == anomalies[start].description) {
                if (i == size - 1) {
                    unified.emplace_back(anomalies[start].timeStep, anomalies[i].timeStep);
                }
                continue;
            } else {
                unified.emplace_back(anomalies[start].timeStep, anomalies[i - 1].timeStep);
                start = i;
                i++;
            }
        }
        return unified;
    }

    void execute() override {
        string Tp = "True Positive Rate: ";
        string Fp = "\nFalse Positive Rate: ";
        vector<pair<int, int>> reports{};
        string line;
        int num1, num2;

        dio->write("Please upload your local anomalies file.\n");
        while (true) {
            line = dio->read();
            if (line == "done")
                break;
            int pos = line.find(",");
            if (pos == line.npos)
                continue;
            stringstream ss1(line.substr(0, pos));
            ss1 >> num1;
            stringstream ss2(line.substr(pos + 1));
            ss2 >> num2;
            reports.emplace_back(num1, num2);
        }
        dio->write("Upload complete.\n");
        vector<pair<int, int>> anomalies = unifyAnom();
        int anomaliesVLen = anomalies.size();
        int n = init->getTimeSteps();
        int P = reports.size();
        int N = n;
        for (int i = 0; i < P; i++) {
            int lines = reports.at(i).second - reports.at(i).first + 1;
            N -= lines;
        }

        int TP = 0;
        for (int j = 0; j < P; j++) {
            pair<int, int> report = reports[j];
            for (int k = 0; k < anomaliesVLen; k++) {
                pair<int, int> anomaly = anomalies[k];
                if ((report.first >= anomaly.first && report.first <= anomaly.second)
                    || (report.second <= anomaly.second && report.second >= anomaly.first)
                    || (report.first <= anomaly.first && report.second >= anomaly.second)) {
                    TP++;
                    break;
                }
            }
        }
        float TP_rate, FP_rate;
        int FP = anomaliesVLen - TP;
        TP_rate = (float(TP) / float(P));
        TP_rate *= 1000;
        TP_rate = floor(TP_rate);
        TP_rate = (TP_rate / 1000);
        FP_rate = (float(FP) / float(N));
        FP_rate *= 1000;
        FP_rate = floor(FP_rate);
        FP_rate = (FP_rate / 1000);
        dio->write("" + Tp);
        dio->write(TP_rate);
        dio->write("" + Fp);
        dio->write(FP_rate);
        dio->write("\n");

    }
};

class DisplayResults : public Command {
public:
    DisplayResults(DefaultIO *dio, initialization *init) : Command(dio, init, "display results") {}

    void execute() override {
        vector<AnomalyReport> res = init->getAnomalies();
        int size = res.size();
        int desc = 0;
        for (int i = 0; i < size; i++) {
            ++desc;
            int timeStep = res.at(i).timeStep;
            dio->write(timeStep);
            dio->write("\t" + res.at(i).description + "\n");
        }
        dio->write("Done.\n");
    }
};

class DetectAnomalies : public Command {
public:
    DetectAnomalies(DefaultIO *dio, initialization *init) : Command(dio, init, "detect anomalies") {}

    void execute() override {
        init->analyze("anomalyTrain.csv", "anomalyTest.csv");
        dio->write("anomaly detection complete.\n");
    }
};


#endif /* COMMANDS_H_ */