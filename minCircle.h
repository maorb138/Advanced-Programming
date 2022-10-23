/*
 * minCircle.h
 *
 * Author:308540343 , Maor Biton.
 */

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>
#include <assert.h>
#include <algorithm>
#include "anomaly_detection_util.h"


using namespace std;


// ------------ DO NOT CHANGE -----------

class Circle {

public:
    Point center;
    float radius;

    Circle(Point c, float r) : center(c), radius(r) {}
};

// --------------------------------------
float distance2(Point &p1, Point &p2);

bool isContain(Circle &c, Point &p);

bool equal(Point &p1, Point &p2);

Circle createCircle(Point &p1, Point &p2, Point &p3);

Circle makeCircle(Point &p1, Point &p2);

Circle minTrivial(vector<Point> points);

Circle helperMinCircle(Point **&pPoint, vector<Point> point1, int size);

Circle helperMinCircle(Point **&p, vector<Point> point1, int size);

Circle findMinCircle(Point **&points, int size);

Circle helperMinCircle(Point **&pPoint, vector<Point> point1, int size);

Point midPoint(Point &p1, Point &p2);

#endif /* MINCIRCLE_H_ */
