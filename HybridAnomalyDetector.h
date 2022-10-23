/*
 * HybridAnomalyDetector.h
 *
 * Author:308540343 , Maor Biton.
 */
#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
    HybridAnomalyDetector();
    virtual ~HybridAnomalyDetector();
    virtual void learnNormal(const TimeSeries &ts) override;
    virtual vector<AnomalyReport> detect(const TimeSeries &ts) override;


};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
