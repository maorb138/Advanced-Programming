/*
 * timeseries.cpp
 *
 * Author:308540343 , Maor Biton.
 */

#include "timeseries.h"
#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;

TimeSeries::TimeSeries(const char *CSVfileName) {
    string line, colname;
    ifstream file(CSVfileName);
    if (!file.is_open())throw runtime_error("unable to open this file !\n");

    if (file.good()) {
        getline(file, line);
        stringstream s(line);
        while (getline(s, colname, ',')) {
            this->dataList.push_back({colname, vector<float>()});
            if (s.peek() == ' ')s.ignore();
        }
    }

    while (getline(file, line)) {
        stringstream ss(line);
        string data;
        int index = 0;
        while (getline(ss, data, ',')) {
            this->dataList.at(index).second.push_back(stof(data));;
            if (ss.peek() == ',')ss.ignore();
            index++;
        }
    }
    file.close();
}

TimeSeries::~TimeSeries() {}

float *TimeSeries::getLine(int index) const {
    int length = listSize();
    float *x = new float[length];
    for (int i = 0; i < length; ++i) {
        float y = this->dataList.at(index).second.at(i);
        x[i] = y;
    }
    return x;
}

int TimeSeries::numOfCol() const {
    int x = this->dataList.size();
    return x;
}

int TimeSeries::listSize() const {
    int x = this->dataList.at(0).second.size();
    return x;
}

Point **TimeSeries::createPoint(int feature1, int feature2) const {
    int size = this->listSize();
    Point **points = new Point *[size];
    for (int i = 0; i < size; ++i) {
        points[i] = new Point(this->dataList.at(feature1).second.at(i), this->dataList.at(feature2).second.at(i));

    }
    return points;
}

void TimeSeries::freePoints(Point **pPoint) const {
    int size = listSize();
    for (int i = 0; i < size; ++i) {
        delete pPoint[i];
        float last = 0;
    }
    delete pPoint;
}

string TimeSeries::getColname(int column) const {
    string s = this->dataList.at(column).first;
    return s;
}

int TimeSeries::attByIndex(string feature) const {
    int x = -1;
    int size = numOfCol();
    for (int i = 0; i < size; ++i) {
        if (!this->dataList.at(i).first.compare(feature)) {
            x = i;
        }
    }
    return x;
}

string TimeSeries::getAtt(int index) const {
    string s = this->featureList.at(index);
    return s;
}

int TimeSeries::sizeOfAtt() const {
    int size = this->featureList.size();
    return size;
}





