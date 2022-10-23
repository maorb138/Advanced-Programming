/*
 * HybridAnomalyDetector.cpp
 *
 * Author:308540343 , Maor Biton.
 */
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

void HybridAnomalyDetector::learnNormal(const TimeSeries &ts) {
    SimpleAnomalyDetector::learnNormal(ts);
    int size= ts.listSize();
    for (int i = 0; i < cf.size(); ++i) {
        if (cf[i].corrlation<0.9 ){
            int feature1 = ts.attByIndex(cf[i].feature1);
            int feature2 = ts.attByIndex(cf[i].feature2);
            Point** pPoint = ts.createPoint(feature1,feature2);
            Circle c = findMinCircle(pPoint,size);
            cf[i].center = &c.center;
            cf[i].radius= c.radius*1.1;
        }
    }
}
vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts){
    vector<AnomalyReport> reporter;
    int size = ts.listSize();
    for (int i = 0; i < cf.size(); ++i) {
        if (cf[i].center!=NULL){
            int feature1 = ts.attByIndex(cf[i].feature1);
            int feature2 = ts.attByIndex(cf[i].feature2);
            Point** pPoint = ts.createPoint(feature1,feature2);
            for (int j = 0; j < size; ++j) {
                Circle x = Circle(*cf[i].center,cf[i].radius);
                if (!isContain(x,*pPoint[j])){
                    const string description = this->cf.at(i).feature1 + "-" +this->cf.at(i).feature2;
                    reporter.push_back(AnomalyReport(description,j+1));
                }
            }
            ts.freePoints(pPoint);
        }
        else{
            int feature1 = ts.attByIndex(this->cf.at(i).feature1);
            int feature2 = ts.attByIndex(this->cf.at(i).feature2);
            Point** pPoint = ts.createPoint(feature1,feature2);
            for (int j = 0; j < size; ++j) {
                if(dev(*pPoint[j],this->cf.at(i).lin_reg)> this->cf.at(i).threshold*1.2){
                    const string description = this->cf.at(i).feature1 + "-" +this->cf.at(i).feature2;
                    reporter.push_back(AnomalyReport(description,j+1));
                }
            }
            ts.freePoints(pPoint);
        }
    }
    return reporter;
}


