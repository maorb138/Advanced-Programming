/*
 * SimpleAnomalyDetector.cpp
 *
 * Author:308540343 , Maor Biton.
 */

#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {}

void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    int length = ts.listSize();
    int features = ts.numOfCol();
    float corrlation;
    int index;
    const double threshold = 0.5;
    for (int i = 0; i < features; ++i) {
        float lastCorrlation = 0;
        float *line1 = ts.getLine(i);
        for (int j = i + 1; j < features; ++j) {
            if (i == j)continue;
            float *line2 = ts.getLine(j);
            corrlation = abs(pearson(line1, line2, length));
            free(line2);
            if (corrlation > threshold && corrlation > lastCorrlation) {
                lastCorrlation = corrlation;
                index = j;
            }
        }
        if (lastCorrlation > 0) {
            string att1 = ts.getColname(i);
            string att2 = ts.getColname(index);
            correlatedFeatures c = correlatedFeatures{att1, att2, lastCorrlation};
            this->cf.push_back(c);
        }
        free(line1);
    }

    for (int i = 0; i < this->cf.size(); ++i) {
        int feature1 = ts.attByIndex(this->cf.at(i).feature1);
        int feature2 = ts.attByIndex(this->cf.at(i).feature2);
        Point **pPoint = ts.createPoint(feature1, feature2);
        float max = 0;
        Line l;
        l = linear_reg(pPoint, length);
        this->cf.at(i).lin_reg = l;
        for (int j = 0; j < length; ++j) {
            float corr = dev(Point(pPoint[j]->x, pPoint[j]->y), l);
            if (corr > max) {
                max = corr;
            }
        }
        this->cf.at(i).threshold = max;
        ts.freePoints(pPoint);
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    int length = ts.listSize();
    int f = ts.sizeOfAtt();
    vector<AnomalyReport> reporter;
    for (int i = 0; i < this->cf.size(); ++i) {
        int feature1 = ts.attByIndex(this->cf.at(i).feature1);
        int feature2 = ts.attByIndex(this->cf.at(i).feature2);
        Point **pPoint = ts.createPoint(feature1, feature2);
        for (int j = 0; j < length; ++j) {
            if (dev(*pPoint[j], this->cf.at(i).lin_reg) > this->cf.at(i).threshold * 1.2) {
                const string description = this->cf.at(i).feature1 + "-" + this->cf.at(i).feature2;
                reporter.push_back(AnomalyReport(description, j + 1));
            }
        }
    }
    return reporter;
}
void SimpleAnomalyDetector::setCorrelationThreshold(float threshold){
    this->threshold=threshold;
}

