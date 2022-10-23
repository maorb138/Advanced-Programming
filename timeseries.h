/*
 * header timeseries.h
 *
 * Author:308540343 , Maor Biton.
 */

#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include "anomaly_detection_util.h"

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;

class TimeSeries {
    vector<pair<string, vector<float>>> dataList;
    vector<string> featureList;
public:

    TimeSeries(const char *CSVfileName);

    ~TimeSeries();

    float *getLine(int index) const;

    int numOfCol() const;

    int listSize() const;

    Point **createPoint(int feature1, int feature2) const;

    void freePoints(Point **pPoint) const;

    string getColname(int column) const;

    int attByIndex(string feature) const;

    string getAtt(int index) const;

    int sizeOfAtt() const;

    string getDataPoint(Point **point) const;

};


#endif /* TIMESERIES_H_ */
